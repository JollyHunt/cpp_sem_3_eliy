#include <gtest/gtest.h>
#include "sequence/ArraySequence.h"
#include "sequence/ListSequence.h"
#include "sequence/SortSeq.h"
#include "Trie.h"
#include "table/CustomHashTable.h"

TEST(ArraySequenceTest, BasicOperations) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.GetLast(), 20);
    seq.Set(0, 5);
    EXPECT_EQ(seq.GetFirst(), 5);
    seq.RemoveAt(0);
    EXPECT_EQ(seq.Size(), 1);
    EXPECT_EQ(seq.Get(0), 20);
}

TEST(ListSequenceTest, BasicOperations) {
    ListSequence<char> seq;
    seq.Append('a');
    seq.Append('b');
    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.Get(1), 'b');
    seq.Prepend('x');
    EXPECT_EQ(seq.GetFirst(), 'x');
    seq.InsertAt(2, 'y');
    EXPECT_EQ(seq.Get(2), 'y');
    seq.RemoveAt(1);
    EXPECT_EQ(seq.Get(1), 'y');
}

TEST(SortedSequenceTest, AddAndOrder) {
    SortedSequence<int> seq;
    seq.Add(30);
    seq.Add(10);
    seq.Add(20);
    EXPECT_EQ(seq.Size(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(TrieTest, InsertAndCount) {
    Trie<char> trie;
    ArraySequence<char> key1;
    key1.Append('a');
    key1.Append('b');
    trie.insert(key1);

    ArraySequence<char> key2;
    key2.Append('a');
    key2.Append('b');
    trie.insert(key2);

    ArraySequence<char> key3;
    key3.Append('a');
    trie.insert(key3);

    auto results = trie.getFrequentStrings();
    bool found_ab = false, found_a = false;
    for (size_t i = 0; i < results.Size(); ++i) {
        if (results.Get(i).first.Size() == 2 && results.Get(i).second == 2) found_ab = true;
        if (results.Get(i).first.Size() == 1 && results.Get(i).second == 1) found_a = true;
    }
    EXPECT_TRUE(found_ab);
    EXPECT_TRUE(found_a);
}

TEST(CustomHashTableTest, AddAndGet) {
    CustomHashTable<ArraySequence<char>, int> table;
    ArraySequence<char> key;
    key.Append('h');
    key.Append('i');
    table.Add(key, 42);
    EXPECT_TRUE(table.ContainsKey(key));
    EXPECT_EQ(table.Get(key), 42);
}

TEST(CustomHashTableTest, CollisionAndResize) {
    CustomHashTable<ArraySequence<int>, std::string> table(2);
    ArraySequence<int> k1;
    k1.Append(1);
    ArraySequence<int> k2;
    k2.Append(2);
    ArraySequence<int> k3;
    k3.Append(3);

    table.Add(k1, "one");
    table.Add(k2, "two");
    table.Add(k3, "three");

    EXPECT_EQ(table.GetCount(), 3);
    EXPECT_EQ(table.Get(k1), "one");
    EXPECT_EQ(table.Get(k2), "two");
    EXPECT_EQ(table.Get(k3), "three");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}