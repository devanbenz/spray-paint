#include <gtest/gtest.h>
#include <fstream>
#include "../src/huffman.h"
#include "../src/heap/min_heap.h"

class SprayPaintTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_file.open("../tests/test.txt");
        large_test_file.open("../tests/135-0.txt");
        medium_test_file.open("../tests/test_two.txt");
        ASSERT_TRUE(test_file.is_open()) << "Could not open test file";
        ASSERT_TRUE(large_test_file.is_open()) << "Could not open test file";
        ASSERT_TRUE(medium_test_file.is_open()) << "Could not open test file";
    }

    void TearDown() override {
        if (test_file.is_open()) {
            test_file.close();
            ASSERT_TRUE(!test_file.is_open());
        }
        if (large_test_file.is_open()) {
            large_test_file.close();
            ASSERT_TRUE(!large_test_file.is_open());
        }
        if (medium_test_file.is_open()) {
            medium_test_file.close();
            ASSERT_TRUE(!medium_test_file.is_open());
        }
    }

    std::ifstream test_file;
    std::ifstream medium_test_file;
    std::ifstream large_test_file;
};

TEST_F(SprayPaintTest, TestHuffBuildCharMap) {
    ASSERT_TRUE(test_file.good()) << "File stream is not in good state";

    std::string content;
    std::getline(test_file, content);
    ASSERT_EQ("hello", content) << "Test file doesn't contain expected content";

    test_file.clear();
    test_file.seekg(0);

    auto char_m = build_char_map(std::move(test_file));
    ASSERT_FALSE(char_m.empty()) << "Character map is empty";

    auto h = char_m.find('h');
    auto e = char_m.find('e');
    auto l = char_m.find('l');
    auto o = char_m.find('o');

    ASSERT_NE(char_m.end(), h) << "'h' not found in map";
    ASSERT_NE(char_m.end(), e) << "'e' not found in map";
    ASSERT_NE(char_m.end(), l) << "'l' not found in map";
    ASSERT_NE(char_m.end(), o) << "'o' not found in map";

    EXPECT_EQ(1, h->second) << "Incorrect count for 'h'";
    EXPECT_EQ(1, e->second) << "Incorrect count for 'e'";
    EXPECT_EQ(2, l->second) << "Incorrect count for 'l'";
    EXPECT_EQ(1, o->second) << "Incorrect count for 'o'";
}

TEST_F(SprayPaintTest, TestHuffNode) {
    auto node1 = LeafNode(10, 'A');
    auto node2 = LeafNode(12, 'B');
    auto empty_node = InternalNode(5, nullptr, nullptr);
    auto just_right = InternalNode(10, nullptr, std::make_unique<SprayPaintNode>(std::move(node1)));

    ASSERT_EQ(node1.weight(), 10) << "Node was not created";
    ASSERT_EQ(node1.value(), 'A') << "Node was not created";
    ASSERT_EQ(node1.leaf(), true);
    ASSERT_EQ(empty_node.leaf(), false);
    ASSERT_EQ(empty_node.weight(), 5);
    ASSERT_EQ(empty_node.right(), nullptr);
    ASSERT_EQ(empty_node.left(), nullptr);
    ASSERT_EQ(just_right.left(), nullptr);
    auto right_leaf = (LeafNode*)(just_right.right());
    ASSERT_EQ(right_leaf->weight(), node1.weight());
    ASSERT_EQ(right_leaf->value(), node1.value());
    ASSERT_EQ(empty_node.left(), nullptr);

    auto full = InternalNode(5, std::make_unique<SprayPaintNode>(std::move(node1)), std::make_unique<SprayPaintNode>(std::move(node2)));
}

TEST_F(SprayPaintTest, MinHeapTests) {
    auto min_heap = MinHeap<int>(5);
    ASSERT_ANY_THROW(min_heap.is_leaf(-10)) << "should throw if neg";
    ASSERT_NO_THROW(min_heap.put(5)) << "should not throw";
    ASSERT_EQ(min_heap.pop(), 5) << "should pop root node";
    ASSERT_ANY_THROW(min_heap.pop()) << "should throw when there is no data in the heap";

    min_heap.put(5);
    min_heap.put(10);
    min_heap.put(2);

    ASSERT_EQ(min_heap.pop(), 2) << "should have gotten 2 when popping; internal structure should be 2 -> 5 -> 10";
    ASSERT_EQ(min_heap.pop(), 5) << "should have gotten 5 when popping; internal structure should be 2 -> 5 -> 10";
    ASSERT_EQ(min_heap.pop(), 10) << "should have gotten 10 when popping; internal structure should be 2 -> 5 -> 10";

    min_heap.put(5);
    min_heap.put(10);
    min_heap.put(2);
    min_heap.put(4);

    ASSERT_EQ(min_heap.pop(), 2) << "should have gotten 2 when popping; internal structure should be 2 -> 5 -> 10";
    ASSERT_EQ(min_heap.pop(), 4) << "should have gotten 4 when popping; internal structure should be 2 -> 5 -> 10";
    ASSERT_EQ(min_heap.pop(), 5) << "should have gotten 5 when popping; internal structure should be 2 -> 5 -> 10";
    ASSERT_EQ(min_heap.pop(), 10) << "should have gotten 10 when popping; internal structure should be 2 -> 5 -> 10";
}


TEST_F(SprayPaintTest, MaxHeapTests) {
    auto min_heap = MaxHeap<int>(5);
    ASSERT_ANY_THROW(min_heap.is_leaf(-10)) << "should throw if neg";
    ASSERT_NO_THROW(min_heap.put(5)) << "should not throw";
    ASSERT_EQ(min_heap.pop(), 5) << "should pop root node";
    ASSERT_ANY_THROW(min_heap.pop()) << "should throw when there is no data in the heap";

    min_heap.put(5);
    min_heap.put(10);
    min_heap.put(2);

    ASSERT_EQ(min_heap.pop(), 10) << "should have gotten 10 when popping; internal structure should be 10 -> 5 -> 2";
    ASSERT_EQ(min_heap.pop(), 5) << "should have gotten 5 when popping; internal structure should be 10 -> 5 -> 2";
    ASSERT_EQ(min_heap.pop(), 2) << "should have gotten 2 when popping; internal structure should be 10 -> 5 -> 2";

    min_heap.put(5);
    min_heap.put(10);
    min_heap.put(2);
    min_heap.put(4);

    ASSERT_EQ(min_heap.pop(), 10);
    ASSERT_EQ(min_heap.pop(), 5);
    ASSERT_EQ(min_heap.pop(), 4);
    ASSERT_EQ(min_heap.pop(), 2);
}

TEST_F(SprayPaintTest, TestMinHeapWithNode) {
    auto node1 = LeafNode(5, 'a');
    auto node2 = LeafNode(8, 'b');
    auto node3 = LeafNode(2, 'c');

    auto min_heap = MinHeap<SprayPaintNode>(5);

    ASSERT_NO_THROW(min_heap.put(std::move(node1))) << "should not throw for inserting node1";
    ASSERT_NO_THROW(min_heap.put(std::move(node2))) << "should not throw for inserting node2";
    ASSERT_NO_THROW(min_heap.put(std::move(node3))) << "should not throw for inserting node3";

    ASSERT_EQ(min_heap.pop(), node3) << "should return node3";
    ASSERT_EQ(min_heap.pop(), node1) << "should return node1";
    ASSERT_EQ(min_heap.pop(), node2) << "should return node2";
}

TEST_F(SprayPaintTest, TestSprayPaintTree) {
    auto tree = SprayPaintTree(5, 'a');
    ASSERT_EQ(tree.root()->weight(), 5);
    ASSERT_EQ(tree.root()->leaf(), true);

    tree.reset();
    ASSERT_FALSE(tree.root().has_value());

    auto tree2 = SprayPaintTree();
    auto charset_m = build_char_map(std::move(medium_test_file));

    ASSERT_NO_THROW(tree2.register_charset(charset_m));
    ASSERT_NO_THROW(tree2.build());
}