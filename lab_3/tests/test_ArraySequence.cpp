#include "sequence/ArraySequence.h"
#include <gtest/gtest.h>
#include <string>

TEST(ArraySequenceTest, BasicOperations) {
    ArraySequence<int> seq;
    EXPECT_TRUE(seq.IsEmpty());
    EXPECT_EQ(seq.Size(), 0);

    seq.Append(10);
    seq.Append(20);
    seq.Append(30);

    EXPECT_FALSE(seq.IsEmpty());
    EXPECT_EQ(seq.Size(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);

    seq.Set(1, 25);
    EXPECT_EQ(seq.Get(1), 25);
}

TEST(ArraySequenceTest, Prepend) {
    ArraySequence<std::string> seq;
    seq.Prepend("world");
    seq.Prepend("hello");

    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.GetFirst(), "hello");
    EXPECT_EQ(seq.GetLast(), "world");
    EXPECT_EQ(seq.Get(0), "hello");
    EXPECT_EQ(seq.Get(1), "world");
}

TEST(ArraySequenceTest, InsertAt) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(3);
    seq.InsertAt(1, 2);

    EXPECT_EQ(seq.Size(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);

    seq.InsertAt(3, 4);
    EXPECT_EQ(seq.GetLast(), 4);
}

TEST(ArraySequenceTest, RemoveAt) {
    ArraySequence<char> seq;
    seq.Append('a');
    seq.Append('b');
    seq.Append('c');

    seq.RemoveAt(1);

    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.Get(0), 'a');
    EXPECT_EQ(seq.Get(1), 'c');
    EXPECT_EQ(seq.GetLast(), 'c');
}

TEST(ArraySequenceTest, Clear) {
    ArraySequence<double> seq;
    seq.Append(1.1);
    seq.Append(2.2);
    seq.Clear();

    EXPECT_TRUE(seq.IsEmpty());
    EXPECT_EQ(seq.Size(), 0);
}

TEST(ArraySequenceTest, OperatorBracket) {
    ArraySequence<int> seq;
    seq.Append(100);
    seq.Append(200);

    seq[0] = 101;
    EXPECT_EQ(seq[0], 101);
    EXPECT_EQ(seq.Get(0), 101);
}

TEST(ArraySequenceTest, OutOfRangeExceptions) {
    ArraySequence<int> seq;

    EXPECT_THROW(seq.GetFirst(), std::out_of_range);
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
    EXPECT_THROW(seq.Get(0), std::out_of_range);
    EXPECT_THROW(seq.Set(0, 42), std::out_of_range);
    EXPECT_THROW(seq.RemoveAt(0), std::out_of_range);

    seq.Append(10);
    EXPECT_THROW(seq.Get(1), std::out_of_range);
    EXPECT_THROW(seq.RemoveAt(1), std::out_of_range);
    EXPECT_THROW(seq.InsertAt(2, 20), std::out_of_range);
}

TEST(ArraySequenceTest, Constructors) {
    ArraySequence<int> seq1;
    EXPECT_TRUE(seq1.IsEmpty());

    ArraySequence<int> seq2(3);
    EXPECT_EQ(seq2.Size(), 3);
    EXPECT_EQ(seq2.Get(0), 0);

    int arr[] = {10, 20, 30};
    ArraySequence<int> seq3(arr, 3);
    EXPECT_EQ(seq3.Size(), 3);
    EXPECT_EQ(seq3.Get(0), 10);
    EXPECT_EQ(seq3.Get(1), 20);
    EXPECT_EQ(seq3.Get(2), 30);
}

TEST(ArraySequenceTest, PushBack) {
    ArraySequence<std::string> seq;
    seq.push_back("first");
    seq.push_back("second");
    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.GetLast(), "second");
}