#include <cards.hpp>
#include <utils.hpp>

namespace memocpp {

void Card::discover() {
    if(!discovered_) {
        discovered_ = true;
        card_state_ = card_face_; 
        std::lock_guard<std::mutex> lock(*total_moves_mutex_);
        (*total_moves_)++;
    }
}

void Card::hide() {
    if(discovered_) {
        discovered_ = false;
        card_state_ = card_back_; 
    }
}

bool Card::is_discovered() const {
    return discovered_;
}

ft::Component Card::get_card() const {
    return show_;
}

bool Card_Pair::check_matched() const {
    return first->is_discovered() && second->is_discovered();
}

card_pairs_t Cards_x4_Factory::genarate_cards(int* total_moves, std::mutex* total_moves_mutex) const {
    card_pairs_t cards;
    cards.reserve(card_faces_.size());
    for(const auto& card_face : card_faces_)
        cards.emplace_back(card_face, card_face, total_moves, total_moves_mutex);

    return cards;
}

card_pairs_t Cards_x6_Factory::genarate_cards(int* total_moves, std::mutex* total_moves_mutex) const {
    card_pairs_t cards;
    cards.reserve(card_faces_.size());
    for(const auto& card_face : card_faces_)
        cards.emplace_back(card_face, card_face, total_moves, total_moves_mutex);

    return cards;
}

}; // namespcae