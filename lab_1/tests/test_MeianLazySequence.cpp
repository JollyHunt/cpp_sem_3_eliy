#include <gtest/gtest.h>
#include "sequence/Generator.h"
#include "sequence/MedianLazySequence.h"

TEST(MedianLazySequenceTest, NaturalNumbersDouble) {
    auto gen = std::make_shared<NaturalNumbersDoubleGenerator>();
    MedianLazySequence<double> seq(gen);

    EXPECT_DOUBLE_EQ(seq.Get(0), 1.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 1.0);

    EXPECT_DOUBLE_EQ(seq.Get(1), 2.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 1.5);

    EXPECT_DOUBLE_EQ(seq.Get(2), 3.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 2.0);

    EXPECT_DOUBLE_EQ(seq.Get(3), 4.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 2.5);
}

TEST(MedianLazySequenceTest, LinearFunction) {
    auto gen = std::make_shared<LinearFunctionGenerator>();
    MedianLazySequence<double> seq(gen);

    EXPECT_DOUBLE_EQ(seq.Get(0), 5.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 5.0);

    EXPECT_DOUBLE_EQ(seq.Get(1), 7.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 6.0);

    EXPECT_DOUBLE_EQ(seq.Get(2), 9.0);
    EXPECT_DOUBLE_EQ(seq.get_current_median(), 7.0);
}

TEST(MedianLazySequenceTest, ThrowsOnEmptyMedian) {
    MedianLazySequence<double> seq(nullptr);
    EXPECT_THROW(seq.get_current_median(), std::runtime_error);
}