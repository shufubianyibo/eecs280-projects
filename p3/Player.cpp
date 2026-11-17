#include <cassert>
#include <iostream>
#include <array>
#include <algorithm> 
#include "Player.hpp"

using namespace std;

class Simple : public Player
{
private:
    string name;
    vector<Card> hand;  

public:
explicit Simple(const string &name_in) : name(name_in) {}

//EFFECTS returns player's name
  const std::string & get_name() const override{
    return name;
}

//REQUIRES player has less than MAX_HAND_SIZE cards
//EFFECTS  adds Card c to Player's hand
  void add_card(const Card &c) override{
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
}

//REQUIRES round is 1 or 2
//MODIFIES order_up_suit
//EFFECTS If Player wishes to order up a trump suit then return true and
//  change order_up_suit to desired suit.  If Player wishes to pass, then do
//  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
                        int round, Suit &order_up_suit) const override{
    assert(round == 1 || round == 2);

    Suit up_suit = upcard.get_suit();
    Suit next_suit = Suit_next(up_suit); // same color suit
            
    int strong_cards = 0;
    if (round == 1) {
        // Count trump-strong cards of upcard suit
        for (const Card &card : hand){
            if (card.is_trump(up_suit) && card.is_face_or_ace()){
                strong_cards++;
            }
        }
        if (strong_cards >= 2){
            order_up_suit = up_suit;
            return true;
        }
    } 
    else { // round == 2
        if (is_dealer){
            order_up_suit = next_suit;
            return true;
        }
        // Count trump-strong cards of "next" suit
        for (const Card &card : hand){
            if (card.is_trump(next_suit) && card.is_face_or_ace()){
                strong_cards++;
            }
        }
        if (strong_cards >= 1){
            order_up_suit = next_suit;
            return true;
        }
    }
    return false; // Pass
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card &upcard) override{
    assert(hand.size() == MAX_HAND_SIZE);
    assert(!hand.empty());

    Suit trump = upcard.get_suit();

    auto is_less_preferred = [trump](const Card& a, const Card& b) {
        bool a_is_trump = a.is_trump(trump);
        bool b_is_trump = b.is_trump(trump);

        if (!a_is_trump && b_is_trump) {
            return true;
        }
        if (a_is_trump && !b_is_trump) {
            return false;
        }

        return Card_less(a, b, trump);
    };
    
    hand.push_back(upcard); 
    auto worst_card_it = std::min_element(hand.begin(), 
    hand.end(), is_less_preferred);

    hand.erase(worst_card_it);
  }

//REQUIRES Player has at least one card
//EFFECTS  Leads one Card from Player's hand according to their strategy
//  "Lead" means to play the first Card in a trick.  The card
//  is removed the player's hand.
  Card lead_card(Suit trump) override{
    assert(!hand.empty());
    auto is_non_trump = [trump](const Card& c) { 
        return !c.is_trump(trump); 
    };

    auto is_less_preferred = [trump, is_non_trump]
    (const Card& a, const Card& b) {
        bool a_is_nt = is_non_trump(a);
        bool b_is_nt = is_non_trump(b);

        if (!a_is_nt && b_is_nt) {
            return true;
        }
        if (a_is_nt && !b_is_nt) {
            return false;
        }
        return Card_less(a, b, trump);
    };
    auto card_to_lead_it = std::max_element(hand.begin(), 
    hand.end(), is_less_preferred);

    Card card_to_lead = *card_to_lead_it;
    
    hand.erase(card_to_lead_it);
    return card_to_lead;
  }

//REQUIRES Player has at least one card
//EFFECTS  Plays one Card from Player's hand according to their strategy.
//  The card is removed from the player's hand.
  Card play_card(const Card &led_card, Suit trump) override{
    assert(!hand.empty());
    Suit led_suit = led_card.get_suit(trump);
    
    vector<Card> follow_suit_cards;
    vector<int> follow_suit_indices;

    for (size_t i = 0; i < hand.size(); ++i) {
      if (hand[i].get_suit(trump) == led_suit) {
        follow_suit_cards.push_back(hand[i]);
        follow_suit_indices.push_back(i);
      }
    }

    if (!follow_suit_cards.empty()) {
      Card highest_card = follow_suit_cards[0];
      int highest_original_index = follow_suit_indices[0];

      for (size_t i = 1; i < follow_suit_cards.size(); ++i) {
        if (Card_less(highest_card, follow_suit_cards[i], trump)) {
          highest_card = follow_suit_cards[i];
          highest_original_index = follow_suit_indices[i];
        }
      }
      hand.erase(hand.begin() + highest_original_index);
      return highest_card;
    }

    Card lowest_card = hand[0];
    int lowest_index = 0;
    for (size_t i = 1; i < hand.size(); ++i) {
      if (Card_less(hand[i], lowest_card, trump)) {
        lowest_card = hand[i];
        lowest_index = i;
      }
    }
    hand.erase(hand.begin() + lowest_index);
    return lowest_card;
  }
};

// --- HumanPlayer (Correct Implementation) ---

class Human : public Player {
private:
  string name;
  vector<Card> hand;

  void print_hand() const {
    vector<Card> sorted_hand = hand;
    sort(sorted_hand.begin(), sorted_hand.end()); 
    for (size_t i = 0; i < sorted_hand.size(); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << sorted_hand[i] << "\n";
    }
  }

public:
  Human(const string &name_in) : name(name_in) {}

  const string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    print_hand();
    cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
    string decision;
    cin >> decision;

    if (decision == "pass") {
      return false;
    }
    
    order_up_suit = string_to_suit(decision);
    return true;
  }

  void add_and_discard(const Card &upcard) override {
    print_hand();
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n";
    
    int index;
    cin >> index;
    assert(index >= 0 && index < (int)hand.size());

    vector<Card> sorted_hand = hand;
    sort(sorted_hand.begin(), sorted_hand.end());
    Card card_to_discard = sorted_hand[index];

    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (*it == card_to_discard) {
        hand.erase(it);
        break;
      }
    }
    
    hand.push_back(upcard);
  }

  Card lead_card(Suit trump) override {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    
    int index;
    cin >> index;
    assert(index >= 0 && index < (int)hand.size());

    vector<Card> sorted_hand = hand;
    sort(sorted_hand.begin(), sorted_hand.end());
    Card card_to_lead = sorted_hand[index];

    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (*it == card_to_lead) {
        hand.erase(it);
        break;
      }
    }
    
    return card_to_lead;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    
    int index;
    cin >> index;
    assert(index >= 0 && index < (int)hand.size());

    vector<Card> sorted_hand = hand;
    sort(sorted_hand.begin(), sorted_hand.end());
    Card card_to_play = sorted_hand[index];

    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (*it == card_to_play) {
        hand.erase(it);
        break;
      }
    }
    
    return card_to_play;
  }
};
   
//EFFECTS: Returns a pointer to a player with the given name and strategy
Player * Player_factory(const std::string &name, const std::string &strategy){
    if (strategy == "Simple"){
    return new Simple(name);
    }
    else if (strategy == "Human"){
    return new Human(name);
    }
    else{
    assert(false); 
    return nullptr;
    }
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p){
        os << p.get_name();
        return os;
}