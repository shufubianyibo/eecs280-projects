#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <cassert> 
#include <cstdlib> 
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

struct PlayerRoster {
    std::string names[4];
    std::string types[4];
};
struct GameConfig {
    bool shuffle;
    int points_to_win;
    PlayerRoster roster;
};

class Game {
    public:
    Game(Pack &pack_in, bool shuffle_in, int points_in, 
        const PlayerRoster &roster_in);
    ~Game(); 
    void play();

    private:
    std::vector<Player*> players;
    Pack &pack;
    bool shuffle_enabled;
    int points_to_win;

    int team1_score = 0; 
    int team2_score = 0; 
    int dealer_index = 0; 
    int hand_count = 0;

    int next_player(int index) const {return (index + 1) % 4;}
    int get_team(int player_index) const { return (player_index % 2 == 0) ? 1 : 2; }
    void deal_to_player(int player_index, int count);
    void deal();
    Suit make_trump(Card &upcard, int &maker_index); 
    void play_hand(Suit trump, int maker_index);
    void score_hand(int maker_index, int team1_tricks, int team2_tricks);
};

Game::Game(Pack &pack_in, bool shuffle_in, int points_in, const PlayerRoster &roster_in)
    : pack(pack_in), shuffle_enabled(shuffle_in), points_to_win(points_in) {
    for (int i = 0; i < 4; ++i) {
        players.push_back(Player_factory(roster_in.names[i], roster_in.types[i]));
    }
}

Game::~Game() {
    for (Player* p : players) {
        delete p;
    }
}

void Game::deal_to_player(int player_index, int count) {
    for (int i = 0; i < count; ++i) {
        players[player_index]->add_card(pack.deal_one());
    }
}

void Game::deal() {
    // 3-2-3-2
    deal_to_player(next_player(dealer_index), 3);
    deal_to_player(next_player(next_player(dealer_index)), 2);
    deal_to_player(next_player(next_player(next_player(dealer_index))), 3);
    deal_to_player(dealer_index, 2);



    // 2-3-2-3
    deal_to_player(next_player(dealer_index), 2);
    deal_to_player(next_player(next_player(dealer_index)), 3);
    deal_to_player(next_player(next_player(next_player(dealer_index))), 2);
    deal_to_player(dealer_index, 3);

}

Suit Game::make_trump(Card &upcard, int &maker_index) {
    Suit trump_suit;
    bool ordered_up = false;

    // Round 1
    int player_index = next_player(dealer_index);
    for (int i = 0; i < 4; ++i) {
      if (players[player_index]->make_trump(upcard, 
        player_index == dealer_index, 1, trump_suit)) {
        cout << players[player_index]->get_name() << " orders up " 
             << upcard.get_suit() << endl; 
        maker_index = player_index;
        players[dealer_index]->add_and_discard(upcard);
        ordered_up = true;
        break;
      }
      cout << players[player_index]->get_name() << " passes" << endl;
      player_index = next_player(player_index);
    }

    // Round 2
    if (!ordered_up) {
      player_index = next_player(dealer_index);
      for (int i = 0; i < 4; ++i) {
        if (players[player_index]->make_trump(upcard, 
            player_index == dealer_index, 2, trump_suit)) {
          cout << players[player_index]->get_name() << " orders up " 
               << trump_suit << endl;
          maker_index = player_index;
          ordered_up = true;
          break;
        }
        cout << players[player_index]->get_name() << " passes" << endl;
        player_index = next_player(player_index);
      }
    }
    
    assert(ordered_up); // "Screw the dealer"
    return trump_suit;
}

void Game::play_hand(Suit trump, int maker_index) {
    int team1_tricks = 0;
    int team2_tricks = 0;
    int leader_index = next_player(dealer_index);

    for (int trick = 0; trick < 5; ++trick) {
        Card led_card = players[leader_index]->lead_card(trump);
        cout << led_card << " led by " << players[leader_index]->get_name() << endl;

        Card highest_card = led_card;
        int trick_winner_index = leader_index;

        for (int i = 1; i < 4; ++i) {
            int current_player_index = next_player(leader_index + i - 1);
            Card played_card = players[current_player_index]->play_card(led_card, trump);
            cout << played_card << " played by " ;
            cout << players[current_player_index]->get_name() << endl;

            if (Card_less(highest_card, played_card, led_card, trump)) {
                highest_card = played_card;
                trick_winner_index = current_player_index;
            }
        }

        leader_index = trick_winner_index;
        cout << players[leader_index]->get_name(); 
        cout << " takes the trick" << endl; 

        if (get_team(leader_index) == 1) {
            team1_tricks++;
        } else {
            team2_tricks++;
        }
    }
    
    score_hand(maker_index, team1_tricks, team2_tricks);
}


void Game::score_hand(int maker_index, int team1_tricks, int team2_tricks) {
    int maker_team = get_team(maker_index); // 1 or 2
    
    if (maker_team == 1) { 
        if (team1_tricks == 5) {
            cout << players[0]->get_name() << " and " ;
            cout << players[2]->get_name() << " win the hand" << endl;
            cout << "march!" << endl;
            team1_score += 2;
        } else if (team1_tricks >= 3) {
            cout << players[0]->get_name() << " and " ;
            cout << players[2]->get_name() << " win the hand" << endl;
            team1_score += 1;
        } else { 
            cout << players[1]->get_name() << " and " ;
            cout << players[3]->get_name() << " win the hand" << endl;
            cout << "euchred!" << endl;
            team2_score += 2;
        }
    } else { 
        if (team2_tricks == 5) {
            cout << players[1]->get_name() << " and " ;
            cout << players[3]->get_name() << " win the hand" << endl;
            cout << "march!" << endl;
            team2_score += 2;
        } else if (team2_tricks >= 3) {
            cout << players[1]->get_name() << " and " ;
            cout << players[3]->get_name() << " win the hand" << endl;
            team2_score += 1;
        } else { 
            cout << players[0]->get_name() << " and " ;
            cout << players[2]->get_name() << " win the hand" << endl;
            cout << "euchred!" << endl;
            team1_score += 2;
        }
    }
}


void Game::play() {
    while (team1_score < points_to_win && team2_score < points_to_win) {
        cout << "Hand " << hand_count << endl;

        if (shuffle_enabled) {
            pack.shuffle();
        } else {
            pack.reset();
        }
        
        cout << players[dealer_index]->get_name() << " deals" << endl;
        deal();
        
        Card upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;
        int maker_index = 0; 
        Suit trump = make_trump(upcard, maker_index);
        
        play_hand(trump, maker_index);

        cout << players[0]->get_name() << " and " << players[2]->get_name()
             << " have " << team1_score << " points" << endl;
        cout << players[1]->get_name() << " and " << players[3]->get_name()
             << " have " << team2_score << " points" << endl << endl;

        dealer_index = next_player(dealer_index);
        hand_count++;
    }

    if (team1_score >= points_to_win) {
        cout << players[0]->get_name() << " and " ;
        cout << players[2]->get_name() << " win!" << endl;
    } else {
        cout << players[1]->get_name() << " and " ;
        cout << players[3]->get_name() << " win!" << endl;
    }
}



void print_usage_error() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
         << "NAME4 TYPE4" << endl;
}


bool check_args(int argc, char *argv[], GameConfig &config) {
    if (argc != 12) {
        print_usage_error();
        return false;
    }

    std::string shuffle_arg = argv[2];
    if (shuffle_arg == "shuffle") {
        config.shuffle = true;
    } else if (shuffle_arg == "noshuffle") {
        config.shuffle = false; 
    } else {
        print_usage_error();
        return false;
    }

    config.points_to_win = atoi(argv[3]); 
    if (config.points_to_win < 1 || config.points_to_win > 100) {
        print_usage_error();
        return false;
    }

    for (int i = 0; i < 4; ++i) {
        config.roster.names[i] = argv[4 + i * 2]; 
        config.roster.types[i] = argv[5 + i * 2]; 
        if (config.roster.types[i] != "Simple" && config.roster.types[i] != "Human") {
            print_usage_error();
            return false;
        }
    }
    
    return true; 
}




int main(int argc, char *argv[]) {
    // Print command-line arguments
    for(int i = 0; i < argc; ++i) {
        cout << argv[i] << " ";
    }
    cout << endl;

    // Open and check pack file
    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cout << "Error opening " << argv[1] << endl;
        return 1;
    }
    Pack pack(fin);
    fin.close();
    GameConfig config;

    // Declare variables to hold validated arguments
    // bool shuffle;
    // int points_to_win;
    // string names[4];
    // string types[4];

    // Validate all other arguments
    if (!check_args(argc, argv, config)) {
        return 1; 
    }

    // Create and run the game
    Game game(pack, config.shuffle, config.points_to_win, config.roster);
    game.play();

    return 0;
}