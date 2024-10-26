#pragma once
#include <ftxui/dom/elements.hpp>

namespace ft = ftxui;

namespace memocpp {

class Card_Pair;
typedef std::vector<Card_Pair> card_pairs_t;

class utils {
public:
    // Helps formatting text in multiline string such as card faces
    static ft::Elements text_split(const std::string& text);
    
    static void hide_unmatched_cards(const card_pairs_t& card_pairs);
};

}; // namespace