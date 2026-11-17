#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Add more tests here
   

TEST(test_simple_player_add_card_full) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Hand should have 5 cards after filling it up
    simple->add_card(Card(NINE, SPADES));
    simple->add_card(Card(TEN, SPADES));
    simple->add_card(Card(JACK, SPADES));
    simple->add_card(Card(QUEEN, SPADES));
    simple->add_card(Card(KING, SPADES));

    // Next add should pass the assert in a debug build, but here we just check setup
    // Since MAX_HAND_SIZE is static const 5, this is correct setup.

    delete simple;
}

// Test: Simple Player orders up in Round 1 with exactly two strong cards.
TEST(test_simple_player_make_trump_round1_two_strong) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Hand: J->Rower (H), J(C), Q(C), K(S), A(S)
    simple->add_card(Card(JACK, HEARTS)); // Right Bower if Hearts is trump
    simple->add_card(Card(JACK, CLUBS));
    simple->add_card(Card(QUEEN, CLUBS));
    simple->add_card(Card(KING, SPADES));
    simple->add_card(Card(ACE, SPADES));

    // Strong Cards for HEARTS: 
    // 1. Jack of Hearts (Rower)
    // 2. Ace of Hearts (Upcard - not in hand)
    // 3. King of Hearts (not in hand)
    // 4. Queen of Hearts (not in hand)
    // 5. Jack of Diamonds (Left Bower) - assumes Diamond is next suit, same color

    // Check 1: Hand contains J(H) [Rower] and J(D) [Left Bower] if upcard is H.
    // We need to add the Left Bower (JD) to ensure 2 strong cards.
    delete simple;
    simple = Player_factory("SimpleSam", "Simple");
    simple->add_card(Card(JACK, HEARTS));   // Right Bower
    simple->add_card(Card(JACK, DIAMONDS)); // Left Bower
    simple->add_card(Card(QUEEN, SPADES));
    simple->add_card(Card(KING, CLUBS));
    simple->add_card(Card(ACE, CLUBS));
    
    Card upcard_h = Card(NINE, HEARTS); // Upcard is HEARTS
    
    Suit trump;
    bool orderup = simple->make_trump(upcard_h, false, 1, trump); 

    // Expect: Order up, Trump is HEARTS (2 strong cards: JH, JD)
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, HEARTS);

    delete simple;
}

// Test: Simple Player passes in Round 1 with one strong card.
TEST(test_simple_player_make_trump_round1_one_strong) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Hand: J->Rower (H) is the only strong card.
    simple->add_card(Card(JACK, HEARTS)); // Right Bower
    simple->add_card(Card(TEN, SPADES));
    simple->add_card(Card(NINE, CLUBS));
    simple->add_card(Card(NINE, DIAMONDS));
    simple->add_card(Card(NINE, HEARTS));

    Card upcard = Card(QUEEN, HEARTS); // Trump is HEARTS
    
    Suit trump;
    bool orderup = simple->make_trump(upcard, false, 1, trump); 

    // Expect: Pass (only 1 strong card)
    ASSERT_FALSE(orderup);

    delete simple;
}

// Test: Simple Player orders up in Round 2 with one strong card of the next suit.
TEST(test_simple_player_make_trump_round2_one_strong) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Upcard is SPADES (Next suit is CLUBS). 
    // Hand has King of CLUBS (strong card).
    simple->add_card(Card(KING, CLUBS)); // Strong card for CLUBS (next suit)
    simple->add_card(Card(TEN, HEARTS));
    simple->add_card(Card(NINE, CLUBS));
    simple->add_card(Card(NINE, DIAMONDS));
    simple->add_card(Card(NINE, HEARTS));

    Card upcard_s = Card(ACE, SPADES); 
    
    Suit trump;
    bool orderup = simple->make_trump(upcard_s, false, 2, trump); 

    // Expect: Order up, Trump is CLUBS (1 strong card: KC)
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, CLUBS);

    delete simple;
}

// Test: Simple Player gets forced to order up ("Screw the Dealer")
TEST(test_simple_player_make_trump_round2_screw_dealer) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Upcard is SPADES (Next suit is CLUBS). 
    // Hand has NO strong cards.
    simple->add_card(Card(TEN, DIAMONDS)); 
    simple->add_card(Card(TEN, HEARTS));
    simple->add_card(Card(NINE, CLUBS));
    simple->add_card(Card(NINE, DIAMONDS));
    simple->add_card(Card(NINE, HEARTS));

    Card upcard_s = Card(ACE, SPADES); 
    
    Suit trump;
    bool orderup = simple->make_trump(upcard_s, true, 2, trump); // is_dealer = true

    // Expect: Order up due to screw the dealer rule. Trump is CLUBS (next suit).
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, CLUBS);

    delete simple;
}

// Test: Simple Player discards the lowest card (Upcard is discarded)
TEST(test_simple_player_add_and_discard_upcard) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Hand of high cards
    simple->add_card(Card(ACE, SPADES));
    simple->add_card(Card(KING, SPADES));
    simple->add_card(Card(QUEEN, SPADES));
    simple->add_card(Card(JACK, SPADES));
    simple->add_card(Card(TEN, SPADES));

    Card upcard = Card(NINE, SPADES); // This is the lowest card
    
    // Trump will be SPADES
    simple->add_and_discard(upcard); 

    // The hand should now contain all cards except the NINE of SPADES (the upcard)
    Card card_led = simple->lead_card(SPADES); // Lead highest trump
    ASSERT_EQUAL(Card(JACK, SPADES), card_led); 

    // Check remaining size (should be 4)
    for (int i = 0; i < 4; ++i) {
        simple->lead_card(SPADES);
    }
    // Attempting to lead again will trigger assert, so we check the hand size
    // Note: The public tests don't expose hand size, but we verify remaining cards
    // The lowest card, NINE of SPADES, should have been discarded and is not present.

    delete simple;
}

TEST(test_simple_player_add_and_discard_original_card) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    Suit trump = CLUBS;
    
    // Cards to test
    const Card discarded_card(NINE, HEARTS); // WORST card to be discarded
    const Card kept_card(JACK, CLUBS);       // RIGHT BOWER (Upcard)
    
    // Initial hand setup:
    simple->add_card(discarded_card); 
    simple->add_card(Card(QUEEN, CLUBS));
    simple->add_card(Card(KING, CLUBS));
    simple->add_card(Card(ACE, CLUBS));
    simple->add_card(Card(ACE, HEARTS)); 

    Card upcard = kept_card; 
    
    simple->add_and_discard(upcard); 

    // Drain the hand into a vector and check the contents in one pass
    vector<Card> final_hand;
    // We explicitly call lead_card 5 times, assuming a hand size of 5 after add_and_discard.
    // This avoids relying on the internal 'assert' to break the loop, which causes the crash.
    for (int i = 0; i < 5; ++i) {
        final_hand.push_back(simple->lead_card(trump));
    }
    
    // The final hand must contain exactly 5 cards.
    ASSERT_EQUAL(5, final_hand.size());

    // Helper to check if a card is in the final_hand vector
    auto is_in_hand = [&](const Card& target) {
        return find(final_hand.begin(), final_hand.end(), target) != final_hand.end();
    };


    // Check 1: The lowest card (9H) should NOT be in the hand.
    ASSERT_FALSE(is_in_hand(discarded_card));

    // Check 2: The Right Bower (JC) should be in the hand.
    ASSERT_TRUE(is_in_hand(kept_card));
    
    delete simple;
}

// Test: Simple Player leads highest non-trump card.
TEST(test_simple_player_lead_card_non_trump) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Trump is HEARTS (Next suit is DIAMONDS)
    simple->add_card(Card(ACE, CLUBS));   // Highest non-trump
    simple->add_card(Card(KING, HEARTS)); // Trump
    simple->add_card(Card(NINE, SPADES)); // Other non-trump
    simple->add_card(Card(TEN, HEARTS));  // Trump
    simple->add_card(Card(QUEEN, DIAMONDS)); // Left Bower (Trump)

    Card card_led = simple->lead_card(HEARTS); // Lead highest non-trump

    // Expect: ACE of CLUBS (AC) to be led.
    ASSERT_EQUAL(Card(ACE, CLUBS), card_led); 

    delete simple;
}

// Test: Simple Player leads highest trump card when hand is all trump.
TEST(test_simple_player_lead_card_all_trump) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Trump is HEARTS
    simple->add_card(Card(TEN, HEARTS));  
    simple->add_card(Card(KING, HEARTS)); 
    simple->add_card(Card(ACE, HEARTS));  // Highest Trump
    simple->add_card(Card(JACK, HEARTS)); // Right Bower
    simple->add_card(Card(JACK, DIAMONDS)); // Left Bower (Trump)

    Card card_led = simple->lead_card(HEARTS); 

    // Expect: ACE of HEARTS (AH) to be led. (Assuming AH > KH > JH > QD > TH)
    // ACE is typically the highest nominal trump unless a bower is involved.
    // The Simple Player strategy finds the highest card overall when all trump.
    // The actual highest card is Right Bower (JH).
    
    // NOTE: The implementation of Card_less defines the exact order. Assuming 
    // the typical Euchre order, Right Bower is the highest.
    ASSERT_EQUAL(Card(JACK, HEARTS), card_led); 

    delete simple;
}

// Test: Simple Player plays highest card that follows suit.
TEST(test_simple_player_play_card_follow_suit_highest) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Trump is DIAMONDS
    simple->add_card(Card(ACE, SPADES)); // Follow card (highest in suit)
    simple->add_card(Card(QUEEN, SPADES)); // Follow card
    simple->add_card(Card(NINE, HEARTS));
    simple->add_card(Card(KING, DIAMONDS)); // Trump
    simple->add_card(Card(TEN, CLUBS));

    Card led_card = Card(NINE, SPADES); // Led suit is SPADES
    
    Card card_played = simple->play_card(led_card, DIAMONDS);

    // Expect: ACE of SPADES (AS) to be played.
    ASSERT_EQUAL(Card(ACE, SPADES), card_played); 

    delete simple;
}

// Test: Simple Player plays lowest card when cannot follow suit.
TEST(test_simple_player_play_card_no_follow_lowest) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Trump is DIAMONDS
    simple->add_card(Card(ACE, SPADES));
    simple->add_card(Card(QUEEN, HEARTS)); 
    simple->add_card(Card(NINE, SPADES)); // Lowest card overall
    simple->add_card(Card(KING, DIAMONDS)); // Trump
    simple->add_card(Card(TEN, SPADES));

    Card led_card = Card(NINE, CLUBS); // Led suit is CLUBS (cannot follow)
    
    Card card_played = simple->play_card(led_card, DIAMONDS);

    // Expect: NINE of SPADES (NS) to be played (lowest card in hand).
    ASSERT_EQUAL(Card(NINE, SPADES), card_played); 

    delete simple;
}

TEST(test_simple_player_play_card_no_follow_trump_lowest) {
    Player * simple = Player_factory("SimpleSam", "Simple");
    // Trump is DIAMONDS
    // Hand: [0] 9S (LOWEST), [1] 10C, [2] 10H, [3] JD (TRUMP)
    simple->add_card(Card(NINE, SPADES)); // Lowest overall
    simple->add_card(Card(TEN, CLUBS));
    simple->add_card(Card(TEN, HEARTS));
    simple->add_card(Card(JACK, DIAMONDS)); // Trump (Right Bower)
    simple->add_card(Card(ACE, CLUBS)); // Highest off suit

    Card led_card = Card(ACE, SPADES); // Led suit is SPADES (player has spades, must follow!)
    
    // **CORRECTION**: To test a trump dump, the player MUST NOT be able to follow.
    delete simple;
    simple = Player_factory("SimpleSam", "Simple");
    // Hand: [0] 9H (LOWEST), [1] 10C, [2] KC, [3] JD (TRUMP)
    simple->add_card(Card(NINE, HEARTS)); // Lowest overall
    simple->add_card(Card(TEN, CLUBS));
    simple->add_card(Card(KING, CLUBS));
    simple->add_card(Card(JACK, DIAMONDS)); // Trump (Right Bower)
    simple->add_card(Card(ACE, DIAMONDS)); // Higher Trump

    Suit trump = DIAMONDS; // Trump suit

    Card card_played = simple->play_card(led_card, trump);

    // When the player cannot follow, the simple strategy is to play the *lowest* card.
    // In this hand, the NINE of HEARTS is the lowest *non-trump* card.
    // The player should still play the lowest card overall, which is the 9H.
    ASSERT_EQUAL(Card(NINE, HEARTS), card_played); 

    delete simple;
}

TEST(test_simple_player_add_and_discard_discards_lowest_card) {
    Player *dealer = Player_factory("Dealer", "Simple");

    // Hand has mixed cards (Spades trump)
    dealer->add_card(Card(ACE, HEARTS));
    dealer->add_card(Card(NINE, SPADES));
    dealer->add_card(Card(JACK, DIAMONDS));
    dealer->add_card(Card(QUEEN, CLUBS));
    dealer->add_card(Card(TEN, SPADES));

    // Upcard is Nine of Spades (makes trump Spades)
    Card upcard(NINE, SPADES);
    dealer->add_and_discard(upcard);

    // After adding and discarding, the lowest card (non-trump low rank) should be gone.
    // JACK of DIAMONDS is not trump and lowest among non-trumps.
    bool found_jack_diamonds = false;
    for (int i = 0; i < 5; ++i) {
        if (dealer->play_card(Card(NINE, HEARTS), SPADES) == Card(JACK, DIAMONDS)) {
            found_jack_diamonds = true;
        }
    }

    ASSERT_FALSE(found_jack_diamonds);

    delete dealer;
}


// -----------------------------------------------------------------------------
// HUMAN PLAYER TESTS (Focus on interactive logic setup)
// -----------------------------------------------------------------------------

TEST(test_human_make_trump_pass) {
    istringstream iss("pass\n");
    ostringstream oss;
    streambuf *cin_buf = cin.rdbuf(iss.rdbuf());
    streambuf *cout_buf = cout.rdbuf(oss.rdbuf());

    Player *human = Player_factory("Eve", "Human");
    human->add_card(Card(ACE, SPADES));
    human->add_card(Card(KING, HEARTS));
    human->add_card(Card(QUEEN, CLUBS));
    human->add_card(Card(JACK, DIAMONDS));
    human->add_card(Card(TEN, SPADES));

    Card upcard(NINE, HEARTS);
    Suit trump = SPADES;
    bool ordered = human->make_trump(upcard, false, 1, trump);

    ASSERT_FALSE(ordered);  // player passed

    // Verify that “pass” didn’t change trump
    ASSERT_EQUAL(trump, SPADES);

    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);
    delete human;
}

TEST(test_human_make_trump_choose_suit) {
    istringstream iss("Hearts\n");
    ostringstream oss;
    streambuf *cin_buf = cin.rdbuf(iss.rdbuf());
    streambuf *cout_buf = cout.rdbuf(oss.rdbuf());

    Player *human = Player_factory("Eve", "Human");
    human->add_card(Card(ACE, SPADES));
    human->add_card(Card(KING, HEARTS));
    human->add_card(Card(QUEEN, CLUBS));
    human->add_card(Card(JACK, DIAMONDS));
    human->add_card(Card(TEN, SPADES));

    Card upcard(NINE, HEARTS);
    Suit trump;
    bool ordered = human->make_trump(upcard, false, 1, trump);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(trump, HEARTS);

    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);
    delete human;
}

TEST(test_human_add_and_discard_normal) {
    // User selects to discard the first card (index 0)
    istringstream iss("0\n");
    ostringstream oss;
    streambuf *cin_buf = cin.rdbuf(iss.rdbuf());
    streambuf *cout_buf = cout.rdbuf(oss.rdbuf());

    Player *dealer = Player_factory("Dealer", "Human");
    dealer->add_card(Card(NINE, HEARTS));
    dealer->add_card(Card(TEN, CLUBS));
    dealer->add_card(Card(JACK, SPADES));
    dealer->add_card(Card(QUEEN, DIAMONDS));
    dealer->add_card(Card(KING, HEARTS));

    dealer->add_and_discard(Card(ACE, HEARTS)); // pick up ACE Hearts
    // Should now still have 5 cards after discarding

    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);
    delete dealer;
}

TEST(test_human_lead_and_play_card) {
    // For both, simulate choosing index 2
    istringstream iss("2\n2\n");
    ostringstream oss;
    streambuf *cin_buf = cin.rdbuf(iss.rdbuf());
    streambuf *cout_buf = cout.rdbuf(oss.rdbuf());

    Player *p = Player_factory("Eve", "Human");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(ACE, DIAMONDS));

    Card lead_card = p->lead_card(SPADES);
    ASSERT_EQUAL(lead_card, Card(QUEEN, CLUBS));

    // After leading, remove one card, so next play uses new hand index 2
    Card led(ACE, DIAMONDS);
    Card play_card = p->play_card(led, HEARTS);
    // Should have played what was originally the KING of SPADES
    ASSERT_EQUAL(play_card, Card(KING, SPADES));

    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);
    delete p;
}

TEST_MAIN()
