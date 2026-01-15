#include <gtest/gtest.h>
#include "bintree/BinTree.h"

TEST(BinTreeTest, EmptyTree) {
    BinTree<double> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_THROW(tree.median(), std::runtime_error);
}

TEST(BinTreeTest, SingleElement) {
    BinTree<double> tree;
    tree.insert(42.0);
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_DOUBLE_EQ(tree.median(), 42.0);
}

TEST(BinTreeTest, TwoElements) {
    BinTree<double> tree;
    tree.insert(10.0);
    tree.insert(20.0);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_DOUBLE_EQ(tree.median(), 15.0);
}

TEST(BinTreeTest, ThreeElements) {
    BinTree<double> tree;
    tree.insert(30.0);
    tree.insert(10.0);
    tree.insert(20.0);
    EXPECT_EQ(tree.size(), 3);
    EXPECT_DOUBLE_EQ(tree.median(), 20.0);
}

TEST(BinTreeTest, Duplicates) {
    BinTree<double> tree;
    tree.insert(5.0);
    tree.insert(5.0);
    tree.insert(5.0);
    EXPECT_EQ(tree.size(), 3);
    EXPECT_DOUBLE_EQ(tree.median(), 5.0);
}

TEST(BinTreeTest, ManyElements) {
    BinTree<double> tree;
    std::vector<double> values = {1.0, 3.0, 2.0, 4.0, 5.0};
    for (double v : values) {
        tree.insert(v);
    }
    EXPECT_EQ(tree.size(), 5);
    EXPECT_DOUBLE_EQ(tree.median(), 3.0);
}