#include <player.hpp>
#include <game_state.hpp>

namespace memocpp {

void Player::update_score(const card_pairs_t& card_pairs) {
    
    int new_score{};
    for(const auto& card_pair : card_pairs)
        new_score += card_pair.check_matched();

    // To calculate the score
    // we need to add the difference of all the matches including the ones made by the current player(new_score) and 
    // all the points that the players were already able to gain so far.
    score_ += new_score - g_state_.get_player_score(Game_State::PLAYER_1) - g_state_.get_player_score(Game_State::PLAYER_2);
}



int Player::get_score() const {
    return score_;
}

ft::Element Player::get_element() const {
    const std::string player_display_name = "Player ";
    const std::string player_display_name_delim = ": ";
    const ft::Color active_player_color = ft::Color::Blue;
    const ft::Color non_active_player_color = ft::Color::White;
    return ft::vbox({
        ft::separator(),
        ft::text(player_display_name + std::to_string(player_ + 1) + player_display_name_delim + std::to_string(score_)),
        ft::separator(),
    })
    | ft::color(g_state_.get_current_player() == player_ ? active_player_color : non_active_player_color);
}

std::string Player::get_winner_splash() const {
    return winner_splash_;
}

}; // namespace