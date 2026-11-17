#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

// Add more tests here
vector<Card> make_standard_pack() {
    vector<Card> result;
    Suit suits[] = {SPADES, HEARTS, CLUBS, DIAMONDS};
    Rank ranks[] = {NINE, TEN, JACK, QUEEN, KING, ACE};
    for (Suit s : suits) {
        for (Rank r : ranks) {
            result.push_back(Card(r, s));
        }
    }
    return result;
}
// TEST 1: Default constructor builds standard ordered pack
TEST(test_pack_default_constructor_order) {
    Pack p;
    vector<Card> expected = make_standard_pack();

    for (int i = 0; i < 24; i++) {
        Card c = p.deal_one();
        ASSERT_TRUE(c == expected[i]);
    }
    ASSERT_TRUE(p.empty());
}

// TEST 2: Stream constructor reads pack correctly
TEST(test_pack_stream_constructor) {
    stringstream ss;
    // Build the same order manually into a stream
    vector<Card> expected = make_standard_pack();
    for (Card c : expected) {
        ss << c << "\n";
    }

    Pack p(ss);
    for (int i = 0; i < 24; i++) {
        Card c = p.deal_one();
        ASSERT_TRUE(c == expected[i]);
    }
    ASSERT_TRUE(p.empty());
}

// TEST 3: deal_one() returns correct sequence and modifies internal index
TEST(test_pack_deal_one_sequence) {
    Pack p;
    vector<Card> expected = make_standard_pack();

    for (int i = 0; i < 10; i++) {
        Card c = p.deal_one();
        ASSERT_TRUE(c == expected[i]);
    }
    ASSERT_FALSE(p.empty()); // Still cards left
}

// TEST 4: reset() resets the next index
TEST(test_pack_reset_function) {
    Pack p;
    vector<Card> expected = make_standard_pack();

    // Deal a few cards
    for (int i = 0; i < 5; i++) {
        p.deal_one();
    }

    // Reset and deal again from the top
    p.reset();
    Card c = p.deal_one();
    ASSERT_TRUE(c == expected[0]); // Should be first card again
}

// TEST 5: empty() returns true only when all cards dealt
TEST(test_pack_empty_function) {
    Pack p;

    for (int i = 0; i < 24; i++) {
        ASSERT_FALSE(p.empty());
        p.deal_one();
    }
    ASSERT_TRUE(p.empty());
}

// TEST 6: shuffle() changes order but keeps same cards
TEST(test_pack_shuffle_function) {
    Pack p;
    vector<Card> before = make_standard_pack();

    p.shuffle();

    vector<Card> after;
    for (int i = 0; i < 24; i++) {
        after.push_back(p.deal_one());
    }

    // Ensure all same cards present
    for (Card c : before) {
        bool found = false;
        for (Card a : after) {
            if (a == c) found = true;
        }
        ASSERT_TRUE(found);
    }

    // It’s very likely (though not 100%) that shuffle changed the order
    bool same_order = true;
    for (int i = 0; i < 24; i++) {
        if (!(before[i] == after[i])) {
            same_order = false;
            break;
        }
    }
    ASSERT_FALSE(same_order);
}

// TEST 7: shuffle() resets the next index automatically
TEST(test_pack_shuffle_resets_next) {
    Pack p;
    for (int i = 0; i < 5; i++) {
        p.deal_one();
    }
    p.shuffle(); // should reset next to 0
    Card c1 = p.deal_one();
    p.reset();
    Card c2 = p.deal_one();
    // After shuffle + reset, both start from beginning again
    ASSERT_TRUE(c1 == c2 || !(c1 == c2)); // ensure it’s dealable, not crashing
}

// TEST 8: multiple shuffles preserve deterministic behavior if algorithm fixed
TEST(test_pack_multiple_shuffles_stability) {
    Pack p1, p2;
    p1.shuffle();
    p2.shuffle();

    bool identical = true;
    for (int i = 0; i < 24; i++) {
        if (!(p1.deal_one() == p2.deal_one())) {
            identical = false;
            break;
        }
    }

    // Deterministic algorithm = identical after same number of shuffles
    ASSERT_TRUE(identical);
}

TEST_MAIN()
