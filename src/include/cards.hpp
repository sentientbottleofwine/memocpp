#pragma once
#include <utils.hpp>
#include <vector>
#include <mutex>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

namespace ft = ftxui;

namespace memocpp {

class Card {
    int* total_moves_;
    std::mutex* total_moves_mutex_;
    std::string card_face_;
    const ft::ElementDecorator card_size_ = ft::size(ft::WIDTH, ft::EQUAL, 13);
    const std::string card_back_ = R"(    __  ___
   /  |/  /
  / /|_/ /
 / /  / /
/_/  /_/)";
    bool discovered_{};
    std::string card_state_ = card_back_;

    ft::ButtonOption custom_button_opt_ = {
        .transform = [&](const ft::EntryState& s) {
            auto base = ft::vbox(utils::text_split(s.label)) | ft::center | ft::border;
            if(discovered_)
                base |= ft::dim;

            if(s.focused)
                return base | ft::inverted;

            if(s.active)
                return base | ft::bold;

            return base;
        }
    };
    
    ft::Component show_;

public:
    Card(const std::string& card_face, int* total_moves, std::mutex* total_moves_mutex) :
        card_face_(card_face),
        total_moves_(total_moves), 
        total_moves_mutex_(total_moves_mutex) {
            // Make the card the right size
            auto show_button = ft::Button(&card_state_, [&]{ discover(); }, custom_button_opt_);
            show_ = ft::Renderer(show_button, [=, this]{ return  show_button->Render() | card_size_; });
    }

    // Updates the Card state to be discovered
    void discover();

    // Analogic to discover but does not get called by the user
    void hide();

    bool is_discovered() const;

    // Returns the card component with current state of the card
    ft::Component get_card() const;
};

typedef std::vector<std::vector<std::shared_ptr<Card>>> card_matrix_t;

class Card_Pair {
public:
    std::shared_ptr<Card> first;
    std::shared_ptr<Card> second;
    Card_Pair(const std::string& card_face_first,
        const std::string& card_face_second,
        int* total_moves,
        std::mutex* total_moves_mutex) : 
        first(std::make_shared<Card>(card_face_first, total_moves, total_moves_mutex)), 
        second(std::make_shared<Card>(card_face_second, total_moves, total_moves_mutex))
    {}
    bool check_matched() const;
};

typedef std::vector<Card_Pair> card_pairs_t;

class Cards_Factory {
public:
    // Returns a vector of cards of appropriate size 
    virtual card_pairs_t genarate_cards(int* total_moves, std::mutex* total_moves_mutex) const = 0;
};

class Cards_x4_Factory : public Cards_Factory {
    const std::array<const std::string, 8> card_faces_ = {
        "$   $\n"
        " $ $ \n"
        "  $  \n"
        " $ $ \n"
        "$   $\n",

        "  &  \n"
        " &&& \n"
        "&&&&&\n"
        " &&& \n"
        "  &  \n",
        
        " *** \n" 
        "** **\n"
        " * * \n"
        "*****\n",

        "# # #\n"
        " # # \n"
        "  #  \n"
        " # # \n"
        "# # #\n",

        " ↑ ↑ \n"
        " ↓ ↓ \n"
        " ← → \n"
        " ← → \n"
        " B A \n",

        "<body> \n"
        " <h1>  \n"
        "  Hello\n"
        " </h1> \n"
        "</body>\n",

        "def f:\n"
        "print(\n"
        " Hello\n"
        ")     \n"
        "f()   \n",

        " [|] \n"
        "[] []\n"
        " [ ] \n"
        "[] []\n"
        " [|] \n"
    };
    card_pairs_t genarate_cards(int* total_moves, std::mutex* total_moves_mutex) const override;
};

class Cards_x6_Factory : public Cards_Factory {
    const std::array<const std::string, 18> card_faces_ = {
        "$   $\n"
        " $ $ \n"
        "  $  \n"
        " $ $ \n"
        "$   $\n",

        "  &  \n"
        " &&& \n"
        "&&&&&\n"
        " &&& \n"
        "  &  \n",
        
        " *** \n" 
        "** **\n"
        " * * \n"
        "*****\n",

        "# # #\n"
        " # # \n"
        "  #  \n"
        " # # \n"
        "# # #\n",

        " ↑ ↑ \n"
        " ↓ ↓ \n"
        " ← → \n"
        " ← → \n"
        " B A \n",

        "<body> \n"
        " <h1>  \n"
        "  Hello\n"
        " </h1> \n"
        "</body>\n",

        "def f:\n"
        "print(\n"
        " Hello\n"
        ")     \n"
        "f()   \n",

        " [|] \n"
        "[] []\n"
        " [ ] \n"
        "[] []\n"
        " [|] \n",

        "\\|/\n"
        "-o-\n"
        "/|\\\n",
         

        "│ │\n"
        "└|┘\n"
        " o \n"
        "┌|┐\n"
        "│ │\n",

        " ╗ ╔ \n"
        " ║ ║ \n"
        "══ ══\n"
        " ║ ║ \n"
        " ╝ ╚ \n",

        "─────\n"
        "╮╭╮╭╮\n"
        "╰╯╰╯╰\n"
        "─────\n",

        "\\   /\n"
        " \\O/ \n"
        "  T  \n"
        " / \\ \n"
        "/   \\\n",
        
        "         \n"
        "   o  |~>\n"
        "| (\\._[~]\n"
        "|~|) |~~|\n",

        "    \n"
        "(0> \n"
        "//\\ \n"
        "V_/_\n"
        "    \n",

        "  ______\n"
        ".'_.--._`\n"
        ":./ :: \\\n"
        "`-~~~~-')\n",
        
        "   __o \n"
        " _ |/<_\n"
        "(_)| (_)\n",

        "int  \n"
        "main()\n"
        "{std::\n"
        "cout<<\n"
        "Hello}\n"
    };
    card_pairs_t genarate_cards(int* total_moves, std::mutex* total_moves_mutex) const override;
};

}; // namespace