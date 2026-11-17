#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"


TEST(test_stub) {
    BinarySearchTree<int> bst;
    ASSERT_TRUE(bst.empty());
}

TEST(assignment_operator_test) {
    BinarySearchTree<int> bst1;
    bst1.insert(10);
    bst1.insert(5);
    bst1.insert(15);

    BinarySearchTree<int> bst2;
    bst2 = bst1; 

    ASSERT_EQUAL(bst2.size(), 3);
    ASSERT_FALSE(bst2.empty());
}

TEST(copy_constructor_test) {
    BinarySearchTree<int> bst1;
    bst1.insert(20);
    bst1.insert(10);
    bst1.insert(30);

    BinarySearchTree<int> bst2(bst1); 

    ASSERT_EQUAL(bst2.size(), 3);
    ASSERT_FALSE(bst2.empty());
}

TEST(height_test) {
    BinarySearchTree<int> bst;
    ASSERT_EQUAL(bst.height(), 0);

    bst.insert(10);
    ASSERT_EQUAL(bst.height(), 1);

    bst.insert(5);
    bst.insert(15);
    ASSERT_EQUAL(bst.height(), 2);

    bst.insert(3);
    ASSERT_EQUAL(bst.height(), 3);
}

TEST(test_insert_and_size) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    ASSERT_EQUAL(bst.size(), 3);
    ASSERT_FALSE(bst.empty());
}

TEST(traverse_preorder_test) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);

    std::ostringstream os;
    bst.traverse_preorder(os);
    ASSERT_EQUAL(os.str(), "10 5 3 7 15 ");
}

TEST(traverse_inorder_test) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);

    std::ostringstream os;
    bst.traverse_inorder(os);
    ASSERT_EQUAL(os.str(), "3 5 7 10 15 ");
}

TEST(check_sorting_invariant_test) {
    BinarySearchTree<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    ASSERT_TRUE(bst.check_sorting_invariant());
}

TEST(iterator_test) {
    BinarySearchTree<int> bst;
    bst.insert(15);
    bst.insert(10);
    bst.insert(20);

    auto it = bst.begin();
    ASSERT_EQUAL(*it, 10); // Smallest element

    ++it;
    ASSERT_EQUAL(*it, 15);

    ++it;
    ASSERT_EQUAL(*it, 20);

    ++it;
    ASSERT_TRUE(it == bst.end());
}

TEST(star_operator_test) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(2);
    bst.insert(8);

    auto it = bst.find(2);
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 2);

    *it = 3; 
    ASSERT_EQUAL(*it, 3);
}

TEST(arrow_operator_test) {
    struct Point {
        int x;
        int y;
        bool operator<(const Point &other) const {
            return x < other.x; 
        }
    };

    BinarySearchTree<Point> bst;
    bst.insert({1, 2});
    bst.insert({3, 4});

    auto it = bst.find({1, 0}); 
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(it->x, 1);
    ASSERT_EQUAL(it->y, 2);
}

TEST(prefix_postfix_increment_test) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);

    auto it = bst.begin();
    ASSERT_EQUAL(*it, 5);

    auto pre_it = ++it; 
    ASSERT_EQUAL(*pre_it, 10);
    ASSERT_EQUAL(*it, 10);

    auto post_it = it++; 
    ASSERT_EQUAL(*post_it, 10);
    ASSERT_EQUAL(*it, 15);
}

TEST(equality_inequality_test) {
    BinarySearchTree<int> bst;
    bst.insert(1);
    bst.insert(2);

    auto it1 = bst.find(1);
    auto it2 = bst.find(2);
    auto it3 = bst.find(1);

    ASSERT_TRUE(it1 == it3);
    ASSERT_TRUE(it1 != it2);
}

TEST(min_max_element_test) {
    BinarySearchTree<int> bst;
    bst.insert(25);
    bst.insert(15);
    bst.insert(35);
    bst.insert(10);
    bst.insert(20);

    auto min_it = bst.min_element();
    ASSERT_TRUE(min_it != bst.end());
    ASSERT_EQUAL(*min_it, 10);

    auto max_it = bst.max_element();
    ASSERT_TRUE(max_it != bst.end());
    ASSERT_EQUAL(*max_it, 35);
}

TEST(begin_end_test) {
    BinarySearchTree<int> bst;
    bst.insert(20);
    bst.insert(10);
    bst.insert(30);

    auto it = bst.begin();
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 10); // Smallest element

    ++it;
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 20);

    ++it;
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 30);

    ++it;
    ASSERT_TRUE(it == bst.end());
}

TEST(min_greater_than_test) {
    BinarySearchTree<int> bst;
    bst.insert(20);
    bst.insert(10);
    bst.insert(30);
    bst.insert(25);
    bst.insert(35);

    auto it = bst.min_greater_than(22);
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 25);

    it = bst.min_greater_than(35);
    ASSERT_TRUE(it == bst.end());
}

TEST(find_test) {
    BinarySearchTree<int> bst;
    bst.insert(25);
    bst.insert(15);
    bst.insert(35);

    auto it = bst.find(15);
    ASSERT_TRUE(it != bst.end());
    ASSERT_EQUAL(*it, 15);

    it = bst.find(100);
    ASSERT_TRUE(it == bst.end());
}

TEST(insert_test) {
    BinarySearchTree<int> bst;
    auto it1 = bst.insert(50);
    ASSERT_TRUE(it1 != bst.end());
    ASSERT_EQUAL(*it1, 50);

    auto it2 = bst.insert(30);
    ASSERT_TRUE(it2 != bst.end());
    ASSERT_EQUAL(*it2, 30);

    auto it3 = bst.insert(70);
    ASSERT_TRUE(it3 != bst.end());
    ASSERT_EQUAL(*it3, 70);

    ASSERT_EQUAL(bst.size(), 3);
}

TEST(test_copy_constructor_new) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);

    BinarySearchTree<int> bst_copy(bst);

    bst_copy.insert(20);
    *bst_copy.begin() = 99; 
    
    ASSERT_EQUAL(bst.size(), 3);
    ASSERT_EQUAL(*bst.begin(), 5); 
    ASSERT_TRUE(bst.find(20) == bst.end());
    
    ASSERT_EQUAL(bst_copy.size(), 4);
    ASSERT_EQUAL(*bst_copy.begin(), 99);
}

TEST(test_copy_assign_empty) {
    BinarySearchTree<int> empty_bst;
    
    // Test Copy Ctor with empty
    BinarySearchTree<int> copy_of_empty(empty_bst);
    ASSERT_TRUE(copy_of_empty.empty());
    ASSERT_EQUAL(copy_of_empty.size(), 0);

    // Test Assignment with empty
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst = empty_bst; 
    
    ASSERT_TRUE(bst.empty());
    ASSERT_EQUAL(bst.size(), 0);
}

TEST(test_assignment_operator_self_assignment) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    
    BinarySearchTree<int> &ref_bst = bst;
    bst = ref_bst;
    ASSERT_EQUAL(bst.size(), 2);
    ASSERT_TRUE(bst.find(10) != bst.end());
    ASSERT_TRUE(bst.find(5) != bst.end());
}

TEST(test_sorting_invariant_broken) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);

    ASSERT_TRUE(bst.check_sorting_invariant());

    auto it = bst.begin(); 
    *it = 20; 
    ASSERT_FALSE(bst.check_sorting_invariant());
    
    *it = 5;
}

TEST(test_copy_structure_preservation) {
    BinarySearchTree<int> bst;
    bst.insert(2);
    bst.insert(1);
    bst.insert(3);

    BinarySearchTree<int> bst_copy(bst);

    ASSERT_EQUAL(bst_copy.size(), 3);

    std::ostringstream os_orig;
    bst.traverse_preorder(os_orig);

    std::ostringstream os_copy;
    bst_copy.traverse_preorder(os_copy);

    ASSERT_EQUAL(os_orig.str(), os_copy.str());
}

TEST(test_empty_tree_operations) {
    BinarySearchTree<int> empty_bst;
    
    ASSERT_TRUE(empty_bst.empty());
    ASSERT_EQUAL(empty_bst.size(), 0);
    ASSERT_EQUAL(empty_bst.height(), 0);
    ASSERT_TRUE(empty_bst.check_sorting_invariant());
    
    ASSERT_TRUE(empty_bst.min_element() == empty_bst.end());
    ASSERT_TRUE(empty_bst.max_element() == empty_bst.end());
    
    std::ostringstream os;
    empty_bst.traverse_inorder(os);
    ASSERT_EQUAL(os.str(), "");
}

TEST(test_copy_non_full_node) {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(7);

    BinarySearchTree<int> bst_copy(bst);

    std::ostringstream os_orig;
    bst.traverse_preorder(os_orig);
    
    std::ostringstream os_copy;
    bst_copy.traverse_preorder(os_copy);
    
    ASSERT_EQUAL(os_orig.str(), os_copy.str());
    
    ASSERT_EQUAL(bst_copy.size(), 3);
    ASSERT_TRUE(bst_copy.find(10) != bst_copy.end());
    ASSERT_TRUE(bst_copy.find(5) != bst_copy.end());
    ASSERT_TRUE(bst_copy.find(7) != bst_copy.end());
}


TEST_MAIN()
