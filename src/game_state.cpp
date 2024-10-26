#include <game_state.hpp>

namespace memocpp {

int Game_State::get_registered_moves() const {
    return registered_moves_;
}

int Game_State::get_player_score(players player) const {
    return players_.at(player).get_score();
}

Game_State::players Game_State::get_current_player() const {
    if(current_turn_ == &players_.at(PLAYER_2))
        return PLAYER_2;
    else
        return PLAYER_1;
};


ft::Element Game_State::get_player_element(players player) const {
    return players_.at(player).get_element();
}
void Game_State::set_registered_moves(int moves) {
    registered_moves_ = moves;
}

void Game_State::update_turn() {
    if(current_turn_ == &players_.at(PLAYER_1))
        current_turn_ = &players_.at(PLAYER_2);
    else
        current_turn_ = &players_.at(PLAYER_1);
}

void Game_State::update_current_player_score(const card_pairs_t& card_pairs) {
    current_turn_->update_score(card_pairs);
}

bool Game_State::is_end(const card_pairs_t& card_pairs) const {
    for(const auto& card_pair : card_pairs)
        if(!card_pair.check_matched())
            return false;
    
    return true;
}

std::string Game_State::get_winner_splash() const {
    if(get_player_score(PLAYER_1) > get_player_score(PLAYER_2))
        return players_.at(PLAYER_1).get_winner_splash();
    else
        return players_.at(PLAYER_2).get_winner_splash();
}

};