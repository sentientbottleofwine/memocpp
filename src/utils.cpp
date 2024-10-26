#include <utils.hpp>
#include <cards.hpp>
#include <sstream>
#include <vector>

namespace memocpp {

ft::Elements utils::text_split(const std::string& text) {
    ft::Elements output;
    std::stringstream ss(text);
    std::string line;
    output.reserve(text.size());
    while (std::getline(ss, line, '\n'))
        output.emplace_back(ft::paragraph(line));
    return output;
}

void utils::hide_unmatched_cards(const card_pairs_t& card_pairs) {
    for(const auto& card_pair : card_pairs)
        if(!card_pair.check_matched()) {
            // It doesn't matter that a card may be already hidden when hide() is called the Card class handles that case too
            card_pair.first->hide();
            card_pair.second->hide();
        }
}

}; // namespace