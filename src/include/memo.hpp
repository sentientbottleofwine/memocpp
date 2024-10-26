#pragma once
#include <vector>
#include <ftxui/component/component.hpp>

namespace ft = ftxui;

namespace memocpp {

class Card;
class Card_Pair;
class Cards_Factory;
typedef std::vector<Card_Pair> card_pairs_t;
typedef std::vector<std::vector<std::shared_ptr<Card>>> card_matrix_t;

class Memo {
    
public:

    // Prepares the playing field and calls the main loop
    Memo();
private:

    const std::string splash_ = R"(    __  ___                                    
   /  |/  /__  ____ ___  ____  _________  ____ 
  / /|_/ / _ \/ __ `__ \/ __ \/ ___/ __ \/ __ \
 / /  / /  __/ / / / / / /_/ / /__/ /_/ / /_/ /
/_/  /_/\___/_/ /_/ /_/\____/\___/ .___/ .___/ 
                                /_/   /_/      )";
    const ft::LinearGradient splash_gradient_ = ft::LinearGradient()
        .Angle(45)
        .Stop(ft::Color::RGB(122, 162, 247))
        .Stop(ft::Color::RGB(187, 154, 247))
        .Stop(ft::Color::RGB(125, 207, 255));
    int total_moves_{};  // Gets updated periodically
                        // from it's difference with the already registered moves count we calculate the new moves count after an end of a round

    std::mutex total_moves_mutex_;
    std::unique_ptr<Cards_Factory> picked_cards_factory_ = nullptr;

    void pick_board_size();

    ft::Component get_card_components(const card_pairs_t& card_pairs);
    void main_loop(ft::Component board, card_pairs_t& card_pairs);
    static ft::Component custom_grid_component(const std::vector<ft::Components>& lines);
    static card_matrix_t shuffle_cards(const card_pairs_t& card_pairs);
};

} // namespace

