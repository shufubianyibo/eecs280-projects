#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here
TEST(test_constructors) {

    // Default Constructor
    Card default_card;
    ASSERT_TRUE(default_card.get_rank() == TWO);
    ASSERT_TRUE(default_card.get_suit() == SPADES);

    // Value Constructor
    Card ace_of_hearts(ACE, HEARTS);
    ASSERT_TRUE(ace_of_hearts.get_rank() == ACE);
    ASSERT_TRUE(ace_of_hearts.get_suit() == HEARTS);
}

TEST(test_getters_and_helpers) {

    Card jack_of_diamonds(JACK, DIAMONDS);
    Card two_of_clubs(TWO, CLUBS);
    Card ace_of_hearts(ACE, HEARTS);
    Card ten_of_diamonds(TEN, DIAMONDS);

    // get_rank() and get_suit()
    ASSERT_TRUE(jack_of_diamonds.get_rank() == JACK);
    ASSERT_TRUE(jack_of_diamonds.get_suit() == DIAMONDS);

    // is_face_or_ace()
    ASSERT_TRUE(jack_of_diamonds.is_face_or_ace()); 
    ASSERT_TRUE(ace_of_hearts.is_face_or_ace());    
    ASSERT_TRUE(!two_of_clubs.is_face_or_ace());    

    // is_right_bower()
    ASSERT_TRUE(jack_of_diamonds.is_right_bower(DIAMONDS)); 
    ASSERT_TRUE(!jack_of_diamonds.is_right_bower(HEARTS));  

    // is_left_bower()
    ASSERT_TRUE(jack_of_diamonds.is_left_bower(HEARTS));
    ASSERT_TRUE(!jack_of_diamonds.is_left_bower(DIAMONDS)); 

    // is_trump()
    ASSERT_TRUE(jack_of_diamonds.is_trump(DIAMONDS)); 
    ASSERT_TRUE(jack_of_diamonds.is_trump(HEARTS));   
    ASSERT_TRUE(!two_of_clubs.is_trump(DIAMONDS));    

    // get_suit(trump) - Left Bower check
    ASSERT_TRUE(jack_of_diamonds.get_suit(HEARTS) == HEARTS);  
    ASSERT_TRUE(two_of_clubs.get_suit(DIAMONDS) == CLUBS);    
    ASSERT_TRUE(jack_of_diamonds.get_suit(DIAMONDS) == DIAMONDS); 

    Suit trump = HEARTS;
    // 1. Left Bower must return the trump suit (Hearts)
    ASSERT_TRUE(jack_of_diamonds.get_suit(trump) == HEARTS);
    
    // 2. A normal card of the same color/suit must return its own suit (Diamonds)
    ASSERT_TRUE(ten_of_diamonds.get_suit(trump) == DIAMONDS);
    
}

TEST(test_card_equality_operator_both_rank_and_suit) {
    Card jack_hearts(JACK, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card queen_hearts(QUEEN, HEARTS);
    Card another_jack_hearts(JACK, HEARTS);

    // Same rank but different suit → not equal
    ASSERT_FALSE(jack_hearts == jack_diamonds);

    // Same suit but different rank → not equal
    ASSERT_FALSE(jack_hearts == queen_hearts);

    // Exactly same rank & suit → equal
    ASSERT_TRUE(jack_hearts == another_jack_hearts);
}



TEST(test_suit_next) {
    ASSERT_TRUE(Suit_next(SPADES) == CLUBS);
    ASSERT_TRUE(Suit_next(CLUBS) == SPADES);
    ASSERT_TRUE(Suit_next(HEARTS) == DIAMONDS);
    ASSERT_TRUE(Suit_next(DIAMONDS) == HEARTS);
}

TEST(test_stream_operators) {
    // operator<<
    Card king_of_spades(KING, SPADES);
    stringstream ss_out;
    ss_out << king_of_spades;
    ASSERT_TRUE(ss_out.str() == "King of Spades");

    // operator>>
    Card read_card;
    stringstream ss_in("Ten of Clubs");
    ss_in >> read_card;
    ASSERT_TRUE(read_card.get_rank() == TEN);
    ASSERT_TRUE(read_card.get_suit() == CLUBS);

    stringstream ss_in_2("Ace of Hearts");
    ss_in_2 >> read_card;
    ASSERT_TRUE(read_card.get_rank() == ACE);
    ASSERT_TRUE(read_card.get_suit() == HEARTS);
}

TEST(test_comparison_operators) {

    Card nine_spades(NINE, SPADES);
    Card ten_spades(TEN, SPADES);
    Card ten_spades_copy(TEN, SPADES);
    Card ten_hearts(TEN, HEARTS);

    // Strict rank ordering (same suit)
    ASSERT_TRUE(nine_spades < ten_spades);
    ASSERT_TRUE(nine_spades <= ten_spades);
    ASSERT_TRUE(ten_spades > nine_spades);
    ASSERT_TRUE(ten_spades >= nine_spades);

    // Equality must hold
    ASSERT_TRUE(ten_spades == ten_spades_copy);
    ASSERT_FALSE(ten_spades != ten_spades_copy);

    // Different suits are NOT comparable for equality
    ASSERT_FALSE(ten_spades == ten_hearts);
    ASSERT_TRUE(ten_spades != ten_hearts);

}

TEST(test_card_less_trump) {
    Suit trump = HEARTS;
    Card right_bower(JACK, HEARTS); // Rank=JACK, Suit=HEARTS. Trump suit.
    Card left_bower(JACK, DIAMONDS);  // Rank=JACK, Suit=DIAMONDS (next). Trump suit.
    Card other_jack(JACK, SPADES);  // Rank=JACK, Suit=SPADES. Not trump.
    Card ace_trump(ACE, HEARTS);    // Rank=ACE, Suit=HEARTS. Trump suit, but low.
    Card ace_off(ACE, CLUBS);       // Rank=ACE, Suit=CLUBS. Not trump.

    // 1. Right Bower > Left Bower > Other Trump > Off Suit
    ASSERT_TRUE(Card_less(ace_trump, right_bower, trump)); // Trump < Right Bower
    ASSERT_TRUE(Card_less(left_bower, right_bower, trump)); // Left Bower < Right Bower
    ASSERT_TRUE(Card_less(other_jack, right_bower, trump)); // Off Suit < Right Bower (Right Bower is trump)
    ASSERT_TRUE(Card_less(ace_off, right_bower, trump));    // Off Suit < Right Bower

    // 2. Left Bower > Other Trump
    ASSERT_TRUE(Card_less(ace_trump, left_bower, trump)); // Trump < Left Bower
    ASSERT_TRUE(!Card_less(left_bower, ace_trump, trump)); // Left Bower is higher

    // 3. Trump vs Off Suit
    ASSERT_TRUE(Card_less(ace_off, ace_trump, trump)); // Off Suit < Trump
    ASSERT_TRUE(!Card_less(ace_trump, ace_off, trump)); // Trump > Off Suit

    // 4. Two Off Suits (Rank comparison)
    ASSERT_TRUE(Card_less(other_jack, ace_off, trump)); // Jack < Ace (both off-suit)

    // 5. Two Trumps (Rank comparison, excluding bower)
    Card king_trump(KING, HEARTS);
    ASSERT_TRUE(Card_less(king_trump, ace_trump, trump)); // King < Ace (both trump, not bower)
    ASSERT_TRUE(Card_less(king_trump, left_bower, trump)); // King < Left Bower (trump vs bower)

}

TEST(test_card_less_led) {

    Suit trump = DIAMONDS; 
    Card led_card(NINE, CLUBS);

    Card right_bower(JACK, DIAMONDS);
    Card left_bower(JACK, HEARTS);

    Card club_ace(ACE, CLUBS);      // Led suit
    Card club_king(KING, CLUBS);    // Led suit
    Card spade_ace(ACE, SPADES);    // Off suit, not led
    Card diamond_ace(ACE, DIAMONDS); // Trump suit (not bower)

    // 1. Trump (Right/Left Bower) > Led > Off Suit (Tested in Card_less(trump) section, but ensuring it holds)
    ASSERT_TRUE(Card_less(club_ace, right_bower, led_card, trump)); // Led < Right Bower
    ASSERT_TRUE(Card_less(club_ace, left_bower, led_card, trump));  // Led < Left Bower
    ASSERT_TRUE(Card_less(spade_ace, right_bower, led_card, trump)); // Off < Right Bower

    // 2. Led Suit vs. Off Suit (Led suit is higher)
    ASSERT_TRUE(Card_less(spade_ace, club_ace, led_card, trump)); // Off Suit < Led Suit
    ASSERT_TRUE(!Card_less(club_ace, spade_ace, led_card, trump)); // Led Suit > Off Suit

    // 3. Two Led Suits (Rank comparison)
    ASSERT_TRUE(Card_less(club_king, club_ace, led_card, trump)); // King < Ace (both led)

    // 4. Two Off Suits (Rank comparison)
    Card spade_king(KING, SPADES);
    ASSERT_TRUE(Card_less(spade_king, spade_ace, led_card, trump)); // King < Ace (both off suit/not trump)

    // 5. Two Trumps (Bower order)
    ASSERT_TRUE(Card_less(diamond_ace, left_bower, led_card, trump)); // Trump < Left Bower
    ASSERT_TRUE(Card_less(left_bower, right_bower, led_card, trump)); // Left Bower < Right Bower

}

TEST(test_card_less_right_and_left_bower_ordering) {
    // Trump = Hearts
    Card right_bower(JACK, HEARTS);   // Jack of trump suit
    Card left_bower(JACK, DIAMONDS);  // Same color Jack (should also be trump)
    Card ace_trump(ACE, HEARTS);
    Card king_trump(KING, HEARTS);

    // Left and Right Bower are both trump
    ASSERT_TRUE(right_bower.is_trump(HEARTS));
    ASSERT_TRUE(left_bower.is_trump(HEARTS));

    // Right Bower should outrank Left Bower
    ASSERT_TRUE(Card_less(left_bower, right_bower, HEARTS));
    ASSERT_FALSE(Card_less(right_bower, left_bower, HEARTS));

    // Both bowers outrank Ace of trump
    ASSERT_TRUE(Card_less(ace_trump, left_bower, HEARTS));
    ASSERT_TRUE(Card_less(ace_trump, right_bower, HEARTS));

    // Regular trumps like King < Ace < Left Bower < Right Bower
    ASSERT_TRUE(Card_less(king_trump, ace_trump, HEARTS));
    ASSERT_TRUE(Card_less(ace_trump, left_bower, HEARTS));
}



TEST_MAIN()
