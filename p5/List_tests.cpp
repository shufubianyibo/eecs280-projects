#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

TEST(test_stub) {
    List<int> l;
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(l.size(), 0);
}

TEST(constructors) {
    List<int> l;
    ASSERT_TRUE(l.empty());
    ASSERT_EQUAL(l.size(), 0);

    List<int> list1;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);

    List<int> list2 = list1; 

    ASSERT_EQUAL(list2.size(), 3);
    auto it = list2.begin();
    ASSERT_EQUAL(*it, 1);
    ++it;
    ASSERT_EQUAL(*it, 2);
    ++it;
    ASSERT_EQUAL(*it, 3);
}

TEST(push_pop_front_back) {
    List<int> l;
    ASSERT_TRUE(l.empty());

    l.push_front(10);
    ASSERT_TRUE(l.front() == 10);
    ASSERT_TRUE(l.back() == 10);
    ASSERT_TRUE(l.size() == 1);

    l.push_back(20);
    ASSERT_TRUE(l.front() == 10);
    ASSERT_TRUE(l.back() == 20);
    ASSERT_TRUE(l.size() == 2);

    l.push_front(5);
    ASSERT_TRUE(l.front() == 5);
    ASSERT_TRUE(l.back() == 20);
    ASSERT_TRUE(l.size() == 3);

    l.pop_front();
    ASSERT_TRUE(l.front() == 10);
    ASSERT_TRUE(l.back() == 20);
    ASSERT_TRUE(l.size() == 2);

    l.pop_back();
    ASSERT_TRUE(l.front() == 10);
    ASSERT_TRUE(l.back() == 10);
    ASSERT_TRUE(l.size() == 1);

    l.pop_back();
    ASSERT_TRUE(l.empty());
}

TEST(assignment_operator) {
    List<int> list1;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);

    List<int> list2;
    list2.push_back(4);
    list2.push_back(5);

    list2 = list1; 

    ASSERT_EQUAL(list2.size(), 3);
    auto it = list2.begin();
    ASSERT_EQUAL(*it, 1);
    ++it;
    ASSERT_EQUAL(*it, 2);
    ++it;
    ASSERT_EQUAL(*it, 3);
}

TEST(clear){
    List<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    ASSERT_TRUE(l.size() == 3);

    l.clear();
    ASSERT_TRUE(l.empty());
    ASSERT_TRUE(l.size() == 0);
}

TEST(insert_erase) {
    List<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    ASSERT_TRUE(l.size() == 3);

    auto it_new_front = l.insert(l.begin(), 5);
    ASSERT_TRUE(*it_new_front == 5);
    ASSERT_TRUE(l.front() == 5);
    ASSERT_TRUE(l.size() == 4);

    auto it_20 = l.begin();
    ++it_20; // Points to 10
    ++it_20; // Points to 20
    auto it_new_mid = l.insert(it_20, 15);
    ASSERT_TRUE(*it_new_mid == 15);
    ASSERT_TRUE(l.size() == 5);

    int expected_mid[] = {5, 10, 15, 20, 30};
    int i = 0;
    for (auto it = l.begin(); it != l.end(); ++it, ++i) {
        ASSERT_TRUE(*it == expected_mid[i]);
    }

    auto it_after_erase = l.erase(it_new_mid);
    ASSERT_TRUE(*it_after_erase == 20); // Should point to 20
    ASSERT_TRUE(l.size() == 4);

    auto it_30 = l.end();
    --it_30; // Points to 30
    it_after_erase = l.erase(it_30);
    ASSERT_TRUE(it_after_erase == l.end()); // Should point to end()
    ASSERT_TRUE(l.back() == 20); // New back is 20
    ASSERT_TRUE(l.size() == 3);
}

TEST(forward_iterator) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    int expected_value = 1;
    for (auto it = l.begin(); it != l.end(); ++it) {
        ASSERT_TRUE(*it == expected_value);
        expected_value++;
    }
    ASSERT_TRUE(expected_value == 4); // Should have iterated through 1, 2, 3
}

TEST(backward_iterator) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    auto it = l.end();
    int expected_value = 3;
    while (it != l.begin()) {
        --it;
        ASSERT_TRUE(*it == expected_value);
        expected_value--;
    }
    ASSERT_TRUE(expected_value == 0); // Should have iterated through 3, 2, 1
}

TEST(equality_inequality) {
    List<int> l1;
    l1.push_back(1);
    l1.push_back(2);

    List<int> l2;
    l2.push_back(1);
    l2.push_back(2);

    auto it1 = l1.begin();
    auto it2 = l1.begin();
    ASSERT_TRUE(it1 == it2);
    ASSERT_FALSE(it1 != it2);

    ++it2;
    ASSERT_FALSE(it1 == it2);
    ASSERT_TRUE(it1 != it2);

    auto it3 = l2.begin();
    ASSERT_FALSE(it1 == it3); // Different lists
}

TEST(assignment_operator_self_assign) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    // Initial check
    ASSERT_EQUAL(l.size(), 3);
    
    // Perform self-assignment
    List<int> &ref_l = l;
    l = ref_l; 

    ASSERT_FALSE(l.empty());
    ASSERT_EQUAL(l.size(), 3);
    ASSERT_EQUAL(l.front(), 1);

    l.pop_front(); 
    
    ASSERT_EQUAL(l.size(), 2);
    ASSERT_EQUAL(l.front(), 2);
    ASSERT_EQUAL(l.back(), 3);

    l.pop_front();
    l.pop_front();
    ASSERT_TRUE(l.empty());
}

TEST(test_assignment_chained) {
    List<int> list1;
    list1.push_back(1);

    List<int> list2;
    List<int> list3;

    list3 = list2 = list1;

    ASSERT_EQUAL(list1.size(), 1);
    ASSERT_EQUAL(list2.size(), 1);
    ASSERT_EQUAL(list3.size(), 1);
    ASSERT_EQUAL(list3.front(), 1);
}

TEST(default_iterator_comparisons) {
    List<int> l;
    l.push_back(10);
    List<int> l_empty;
    
    List<int>::Iterator it_def1;
    List<int>::Iterator it_def2;
    ASSERT_TRUE(it_def1 == it_def2);
    
    List<int>::Iterator it_begin1 = l.begin();
    List<int>::Iterator it_begin2 = l.begin();
    ASSERT_TRUE(it_begin1 == it_begin2);
    
    List<int>::Iterator it_end = l.end();
    ASSERT_TRUE(it_begin1 != it_end); 
    ASSERT_TRUE(it_def1 != it_end); 
    List<int>::Iterator it_empty_end = l_empty.end();
    ASSERT_TRUE(it_def1 != it_empty_end); 
}

TEST_MAIN()
