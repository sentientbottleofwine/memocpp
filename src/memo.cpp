#include <memo.hpp>
#include <cards.hpp>
#include <game_state.hpp>
#include <utils.hpp>
#include <cmath>
#include <thread>
#include <cassert>
#include <random>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/event.hpp>

namespace ft = ftxui;

namespace memocpp {

Memo::Memo() {
    pick_board_size();
    if(picked_cards_factory_ == nullptr)
        exit(0);

    card_pairs_t card_pairs = picked_cards_factory_->genarate_cards(&total_moves_, &total_moves_mutex_);
    main_loop(get_card_components(card_pairs), card_pairs);
}

void Memo::pick_board_size() {
    int selector;
    const int available_options = 2;
    const ft::Color info_text_color = ft::Color::Cyan;
    const ft::ElementDecorator options_size = ft::size(ft::WIDTH, ft::EQUAL, 7);
    const std::string info_text = "Welcome to my memory game, memocpp. Please pick the desired size of the board size.";
    const std::vector<std::string> board_options {
        "4 X 4",
        "6 x 6"
    };
    std::array<std::unique_ptr<Cards_Factory>, available_options> card_factories = {
        std::make_unique<Cards_x4_Factory>(),
        std::make_unique<Cards_x6_Factory>(),
    };

    auto screen = ft::ScreenInteractive::Fullscreen();
    ft::MenuOption board_menu_opt = {
        .entries = &board_options,
        .selected = &selector,
        .entries_option = {
            .transform = [&](const ft::EntryState& s){ 
                ft::Element base = ft::text(s.label);
                if(s.focused)
                    return base | ft::borderStyled(info_text_color);
                
                return base | ft::border;
            },
        },
        .on_enter = [&]{
            screen.Exit();
            picked_cards_factory_.swap(card_factories.at(selector));
            return;
        },
        .focused_entry = &selector
    };

    auto board_menu = ft::Menu(board_menu_opt)
    | ft::CatchEvent([&](ft::Event event){
        if(event.mouse().button == ft::Mouse::Left && event.mouse().motion == ft::Mouse::Pressed) {
            board_menu_opt.on_enter();
            return true;
        }
        return false;
    });

    auto render = ft::Renderer(board_menu, [&]{
        return ft::vbox({
            ft::vbox(
                utils::text_split(splash_) | ft::color(splash_gradient_),
                ft::separator()
            ) | ft::hcenter,
            ft::paragraph(info_text) | ft::color(info_text_color),
            board_menu->Render() | options_size | ft::hcenter 
        })  | ft::center;
    });

    screen.Loop(render);
}

ft::Component Memo::get_card_components(const card_pairs_t& card_pairs) {
    auto shuffled_cards = shuffle_cards(card_pairs);
    std::vector<ft::Components> horizontal_card_components;
    horizontal_card_components.reserve(shuffled_cards.size());

    for(int i{}; i < shuffled_cards.size(); i++) {
        ft::Components card_row_components;
        card_row_components.reserve(shuffled_cards.size());

        for(int j{}; j < shuffled_cards.size(); j++) // Size of columns is the same as rows
            card_row_components.emplace_back(shuffled_cards.at(i).at(j)->get_card());

        horizontal_card_components.emplace_back(card_row_components);
    }

    return custom_grid_component(horizontal_card_components);
};

void Memo::main_loop(ft::Component board, card_pairs_t& card_pairs) {
    using namespace std::chrono_literals;
    const auto discovered_unmatched_card_wait_time = 1s;
    const int max_player_moves = 2;
    Game_State g_state;
    int is_end{}; // Used as an index in the tab container. Can be only 1 or 0.

    std::unique_ptr<std::thread> hide_cards_thread_handle = nullptr;
    auto screen = ft::ScreenInteractive::Fullscreen();
    auto end_game_component = ft::Renderer([&]{
        const std::string end_game_text = "The game has ended. Press Ctrl+C to quit";
        const ft::Color end_game_text_color = ft::Color::Cyan;
        return ft::vbox({
            ft::vbox(utils::text_split(g_state.get_winner_splash())) | ft::color(splash_gradient_)| ft::border,
            ft::text(end_game_text) | ft::color(end_game_text_color)
        }) | ft::center;
    });

    auto main_game_component = ft::Renderer(board, [&]{ 
        // Join the thread that was hiding the cards in the previous iteration of the loop.
        // This is not a race condition on card_pairs because the main ui thread is blocked until the one that was hiding the cards joins.
        if(hide_cards_thread_handle != nullptr) {
            hide_cards_thread_handle->join();
            hide_cards_thread_handle.reset();
        }

        int current_moves = total_moves_ - g_state.get_registered_moves();
        
        if(current_moves == max_player_moves) { // If the player discovered two cards
            g_state.set_registered_moves(g_state.get_registered_moves() + current_moves);

            int old_score = g_state.get_player_score(g_state.get_current_player());
            g_state.update_current_player_score(card_pairs);
            int new_score = g_state.get_player_score(g_state.get_current_player());
            if(old_score == new_score) {
                // Spawn a new thread so that screen can update
                hide_cards_thread_handle = std::make_unique<std::thread>([&]{ 
                    std::this_thread::sleep_for(discovered_unmatched_card_wait_time);
                    utils::hide_unmatched_cards(card_pairs);
                    g_state.update_turn();
                });
            }

            is_end = g_state.is_end(card_pairs);
            screen.PostEvent(ft::Event::Special("Update screen"));
        }
        return ft::vbox({
            ft::vbox(utils::text_split(splash_)) | ft::color(splash_gradient_),
            ft::vbox({
                g_state.get_player_element(Game_State::PLAYER_1),
                board->Render()
                | ft::vscroll_indicator
                | ft::frame
                | ft::hcenter,
                g_state.get_player_element(Game_State::PLAYER_2)
            })
        }) | ft::center;
    });

    auto render = ft::Container::Tab({
        main_game_component,
        end_game_component
    }, &is_end);

    screen.Loop(render);
}

ft::Component Memo::custom_grid_component(const std::vector<ftxui::Components>& lines) {
    class Impl : public ft::ComponentBase {
    private:
        std::vector<ft::Components> rows_;
        ft::Ref<int> selector_vertical_;
        ft::Ref<int> selector_horizontal_;

    public:
        Impl(std::vector<ft::Components> lines,
        ft::Ref<int> selector_vertical,
        ft::Ref<int> selector_horizontal)
        : rows_(std::move(lines)),
        selector_vertical_(selector_vertical),
        selector_horizontal_(selector_horizontal) {
            ft::Components horizontalComponents_;
            for (const auto& line : rows_) {
              horizontalComponents_.push_back(
                  ft::Container::Horizontal(line, &selector_horizontal_()));
            }
            Add(ft::Container::Vertical(horizontalComponents_, &selector_vertical_()));
        }
    };

    return Make<Impl>(lines, 0, 0);
}

card_matrix_t Memo::shuffle_cards(const card_pairs_t& cards) {
    const int cards_in_pair = 2;
    const int size_of_all_cards = cards.size() * cards_in_pair;
    const int card_line_size = sqrt(size_of_all_cards); // Because size of the board is row/column squared
    std::vector<std::shared_ptr<Card>> unshuffled_cards;
    card_matrix_t shuffled_card_matrix;

    unshuffled_cards.reserve(size_of_all_cards);
    for(const auto& ucard : cards) {
        unshuffled_cards.emplace_back(ucard.first);
        unshuffled_cards.emplace_back(ucard.second);
    }

    std::random_device rd;
    std::mt19937 gen{rd()};

    std::shuffle(unshuffled_cards.begin(), unshuffled_cards.end(), gen);

    shuffled_card_matrix.resize(card_line_size);
    for(auto& card_row : shuffled_card_matrix)
        card_row.reserve(card_line_size);

    // Basically places copies all the card from a 1d vector of size a^2 to a 2d matrix of size a x a
    for(int i{}; i < card_line_size; i++) {
       const int vec_index_start = i * card_line_size;
        for(int j{}; j < card_line_size; j++)
            shuffled_card_matrix.at(i).emplace_back(unshuffled_cards.at(vec_index_start + j));
    }

    return shuffled_card_matrix; 
}

}; // namespace