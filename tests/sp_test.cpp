#include <gtest/gtest.h>
#include <fstream>
#include "../src/huffman.h"
#include "../src/heap/min_heap.h"

class SprayPaintTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_file.open("../tests/test.txt");
        ASSERT_TRUE(test_file.is_open()) << "Could not open test file";
    }

    void TearDown() override {
        if (test_file.is_open()) {
            test_file.close();
            ASSERT_TRUE(!test_file.is_open());
        }
    }

    std::ifstream test_file;
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

TEST_F(SprayPaintTest, TestHuffBuildCharSet) {
    ASSERT_TRUE(test_file.good()) << "File stream is not in a good state";

    std::string content;
    std::getline(test_file, content);
    ASSERT_EQ("hello", content) << "Test file doesn't contain expected content";

    test_file.clear();
    test_file.seekg(0);

    auto char_m = build_char_map(std::move(test_file));
    ASSERT_FALSE(char_m.empty()) << "Character map is empty";

    auto char_s = build_set(char_m);
    ASSERT_FALSE(char_s.empty());

    auto val = char_s.back();
    ASSERT_EQ(val, std::make_pair('l', 2));
}

TEST_F(SprayPaintTest, TestHuffNode) {
    auto node1 = SprayPaintNode(10, 'A', true);

    ASSERT_EQ(node1.is_leaf(), true) << "Node was not created";
    ASSERT_EQ(node1.weight(), 10) << "Node was not created";
    ASSERT_EQ(node1.value(), 'A') << "Node was not created";
    ASSERT_ANY_THROW(node1.insert_node(nullptr))<< "should throw on insertion of nullptr";
}

TEST_F(SprayPaintTest, HeapTests) {
    auto min_heap = MinHeap<int>(5);
    ASSERT_ANY_THROW(min_heap.is_leaf(-10)) << "should throw if neg";
    ASSERT_NO_THROW(min_heap.put(5)) << "should not throw";
    ASSERT_NO_THROW(min_heap.put(7)) << "should not throw";
}
