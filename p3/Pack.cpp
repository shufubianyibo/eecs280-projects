#include <cassert>
#include <iostream>
#include <array>
#include "Pack.hpp"

using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on. 
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(){
    Suit suits[] = {SPADES, HEARTS, CLUBS, DIAMONDS};
    Rank ranks[] = {NINE, TEN, JACK, QUEEN, KING, ACE};
    int index = 0;
    for (Suit suit : suits){
        for (Rank rank : ranks){
        cards[index] = Card(rank, suit);
        index++;
        }
    }
    next = 0;
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(std::istream& pack_input){
    int index = 0;
    while (pack_input&& index < PACK_SIZE) {
        pack_input >> cards[index];
        index++;
    }
    next = 0;
    
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one(){
    assert(!empty());
    Card dealt_card = cards[next];
    next++;
    return dealt_card;
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset(){
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle(){
    std::array<Card, PACK_SIZE> shuffled;
    for (int shuffle_count = 0; shuffle_count < 7; shuffle_count++){
        int mid = PACK_SIZE / 2;
        int left = 0;
        int right = mid;
        for (int i = 0; i < PACK_SIZE; i++){
            if (i % 2 != 0){
                shuffled[i] = cards[left];
                left++;
            }
            else{
                shuffled[i] = cards[right];
                right++;
            }
        }
        cards = shuffled; 
    }
    next = 0; 
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const{
    return (next >= PACK_SIZE);
}