#pragma once
#include <cards.hpp>
#include <ftxui/dom/elements.hpp>

namespace memocpp {

class Game_State;

class Player {
public:
    /// @param g_state Games State object
    /// @param player an enu entry treated as an index
    Player(const Game_State& g_state, int player, const std::string& winner_splash) : 
        g_state_(g_state), 
        player_(player),
        winner_splash_(winner_splash) {}
    int get_score() const;
    ft::Element get_element() const;
    std::string get_winner_splash() const;
    void update_score(const card_pairs_t& cards);

private:
    const std::string& winner_splash_;
    const Game_State& g_state_;
    int player_;
    int score_{};
};

} // namespace