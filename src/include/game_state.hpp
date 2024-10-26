#pragma once
#include <unordered_map>
#include <player.hpp>

namespace memocpp {

// Holds core game state
class Game_State {
public:
    enum players {
        PLAYER_1,
        PLAYER_2,
    };

    int get_registered_moves() const;
    int get_player_score(players player) const;
    players get_current_player() const;
    ft::Element get_player_element(players player) const;
    void set_registered_moves(int moves);
    void update_turn();
    void update_current_player_score(const card_pairs_t& card_pairs);
    // Checks if all cards have been discovered
    bool is_end(const card_pairs_t& card_pairs) const;
    std::string get_winner_splash() const;

private:
    static const int player_count_ = 2;
    const std::array<std::string, player_count_> player_splashes_ = {
        R"(
    ____  __    _____  ____________     ___   _       ______  _   ____
   / __ \/ /   /   \ \/ / ____/ __ \   <  /  | |     / / __ \/ | / / /
  / /_/ / /   / /| |\  / __/ / /_/ /   / /   | | /| / / / / /  |/ / / 
 / ____/ /___/ ___ |/ / /___/ _, _/   / /    | |/ |/ / /_/ / /|  /_/  
/_/   /_____/_/  |_/_/_____/_/ |_|   /_/     |__/|__/\____/_/ |_(_)   
        )",
        R"(
    ____  __    _____  ____________     ___      _       ______  _   ____
   / __ \/ /   /   \ \/ / ____/ __ \   |__ \    | |     / / __ \/ | / / /
  / /_/ / /   / /| |\  / __/ / /_/ /   __/ /    | | /| / / / / /  |/ / / 
 / ____/ /___/ ___ |/ / /___/ _, _/   / __/     | |/ |/ / /_/ / /|  /_/  
/_/   /_____/_/  |_/_/_____/_/ |_|   /____/     |__/|__/\____/_/ |_(_)   
        )"
    };
    int registered_moves_{};
    std::array<Player, player_count_> players_ = { 
        Player(*this, PLAYER_1, player_splashes_.at(PLAYER_1)), 
        Player(*this, PLAYER_2, player_splashes_.at(PLAYER_2))
    };

    Player* current_turn_ = &players_.at(PLAYER_1); // Player one begins
    bool end_{};
};

} // namespace

                                                                      

                                                                         
