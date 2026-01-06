#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "../include/sequence/LazySequence.h"
#include "../include/sequence/Generator.h"

class LazySequenceTest : public ::testing::Test {};

TEST_F(LazySequenceTest, DefaultConstructor) {
    LazySequence<int> seq;
    EXPECT_EQ(seq.Size(), 0);
    EXPECT_TRUE(seq.is_finite());
    EXPECT_EQ(seq.GetMaterializedCount(), 0);
}

TEST_F(LazySequenceTest, GeneratorConstructor) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    EXPECT_FALSE(seq.is_finite());
    EXPECT_EQ(seq.Get(0), 0);
    EXPECT_EQ(seq.Get(5), 5);
    EXPECT_EQ(seq.GetMaterializedCount(), 6);
}

TEST_F(LazySequenceTest, ArrayConstructor) {
    int arr[] = {10, 20, 30};
    LazySequence<int> seq(arr, 3);
    EXPECT_TRUE(seq.is_finite());
    EXPECT_EQ(seq.Size(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(2), 30);
    EXPECT_EQ(seq.GetMaterializedCount(), 3);
}

TEST_F(LazySequenceTest, ListSequenceConstructor) {
    int arr[] = {5, 15};
    ListSequence<int> list(arr, 2);
    LazySequence<int> seq(list);
    EXPECT_TRUE(seq.is_finite());
    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.Get(1), 15);
}

TEST_F(LazySequenceTest, GetFirst) {
    auto gen = std::make_shared<ArithmeticProgressionGenerator<int>>(100, 10);
    LazySequence<int> seq(gen);
    EXPECT_EQ(seq.GetFirst(), 100);
}

TEST_F(LazySequenceTest, GetLastFinite) {
    int arr[] = {1, 2, 3, 4, 5};
    LazySequence<int> seq(arr, 5);
    EXPECT_EQ(seq.GetLast(), 5);
}

TEST_F(LazySequenceTest, GetLastInfinite) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    seq.Get(9);
    EXPECT_EQ(seq.GetLast(), 9);
}

TEST_F(LazySequenceTest, SetCapacity) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    seq.set_capacity(3);
    EXPECT_TRUE(seq.is_finite());
    EXPECT_EQ(seq.Size(), 3);
    EXPECT_EQ(seq.Get(2), 2);
    EXPECT_THROW(seq.Get(3), std::out_of_range);
}

TEST_F(LazySequenceTest, SetFinite) {
    LazySequence<int> seq;
    seq.set_finite(true, 2);
    EXPECT_TRUE(seq.is_finite());
    EXPECT_EQ(seq.finite_capacity(), 2);
}

TEST_F(LazySequenceTest, Concat) {
    int a[] = {1, 2};
    int b[] = {3, 4};
    LazySequence<int> s1(a, 2);
    LazySequence<int> s2(b, 2);
    auto res = s1.concat(s2);
    EXPECT_EQ(res->Get(0), 1);
    EXPECT_EQ(res->Get(1), 2);
    EXPECT_EQ(res->Get(2), 3);
    EXPECT_EQ(res->Get(3), 4);
    EXPECT_EQ(res->Size(), 4);
}

TEST_F(LazySequenceTest, Insert) {
    int main_arr[] = {10, 30};
    int ins_arr[] = {20};
    LazySequence<int> main_seq(main_arr, 2);
    LazySequence<int> ins_seq(ins_arr, 1);
    auto res = main_seq.insert(ins_seq, 1);
    EXPECT_EQ(res->Get(0), 10);
    EXPECT_EQ(res->Get(1), 20);
    EXPECT_EQ(res->Get(2), 30);
    EXPECT_EQ(res->Size(), 3);
}

TEST_F(LazySequenceTest, Map) {
    int arr[] = {1, 2, 3};
    LazySequence<int> seq(arr, 3);
    auto doubled = seq.map<int>([](int x) { return x * 2; });
    EXPECT_EQ(doubled->Get(0), 2);
    EXPECT_EQ(doubled->Get(1), 4);
    EXPECT_EQ(doubled->Get(2), 6);
}

TEST_F(LazySequenceTest, Where) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    auto evens = seq.Where([](int x) { return x % 2 == 0; });
    EXPECT_EQ(evens->Get(0), 0);
    EXPECT_EQ(evens->Get(1), 2);
    EXPECT_EQ(evens->Get(2), 4);
}

TEST_F(LazySequenceTest, Zip) {
    int a[] = {1, 2, 3};
    std::string b[] = {"x", "y", "z"};
    LazySequence<int> s1(a, 3);
    LazySequence<std::string> s2(b, 3);
    auto zipped = s1.Zip(s2);
    EXPECT_EQ(zipped.Size(), 3);
    EXPECT_EQ(zipped.Get(0).first, 1);
    EXPECT_EQ(zipped.Get(0).second, "x");
    EXPECT_EQ(zipped.Get(2).first, 3);
    EXPECT_EQ(zipped.Get(2).second, "z");
}

TEST_F(LazySequenceTest, Reduce) {
    int arr[] = {1, 2, 3, 4};
    LazySequence<int> seq(arr, 4);
    int sum = seq.Reduce<int>([](int acc, int x) { return acc + x; }, 0);
    EXPECT_EQ(sum, 10);
}

TEST_F(LazySequenceTest, NaturalNumbersGenerator) {
    auto gen = std::make_shared<NaturalNumbersGenerator<size_t>>();
    LazySequence<size_t> seq(gen);
    for (size_t i = 0; i < 300; ++i) {
        EXPECT_EQ(seq.Get(i), i);
    }
}

TEST_F(LazySequenceTest, ArithmeticProgressionGenerator) {
    auto gen = std::make_shared<ArithmeticProgressionGenerator<double>>(0.5, 1.5);
    LazySequence<double> seq(gen);
    EXPECT_DOUBLE_EQ(seq.Get(0), 0.5);
    EXPECT_DOUBLE_EQ(seq.Get(1), 2.0);
    EXPECT_DOUBLE_EQ(seq.Get(2), 3.5);
}

TEST_F(LazySequenceTest, StringSequenceGenerator) {
    auto gen = std::make_shared<StringSequenceGenerator>();
    LazySequence<std::string> seq(gen);
    for (size_t i = 0; i < 300; ++i) {
        EXPECT_EQ(seq.Get(i), "s" + std::to_string(i));
    }
}

TEST_F(LazySequenceTest, MaterializeUpToBoundary) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    seq.set_finite(true, 10);
    EXPECT_EQ(seq.Get(9), 9);
    EXPECT_THROW(seq.Get(10), std::out_of_range);
}

TEST_F(LazySequenceTest, EmptyFiniteGetLastThrows) {
    LazySequence<int> seq;
    seq.set_finite(true, 0);
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
}

TEST_F(LazySequenceTest, InfiniteGetLastAfterMaterialize) {
    auto gen = std::make_shared<NaturalNumbersGenerator<int>>();
    LazySequence<int> seq(gen);
    seq.Get(5);
    EXPECT_EQ(seq.GetLast(), 5);
}