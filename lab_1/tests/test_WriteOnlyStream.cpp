#include <gtest/gtest.h>
#include "stream/WriteOnlyStream.h"
#include "sequence/ListSequence.h"
#include <string>

TEST(WriteOnlyStreamTest, ToSequence) {
    ListSequence<int> seq;
    WriteOnlyStream<int> stream(&seq, DataType::INT);
    stream.Write(10);
    stream.Write(20);
    EXPECT_EQ(seq.Size(), 2);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(WriteOnlyStreamTest, ToString) {
    std::string output;
    WriteOnlyStream<int> stream(output, DataType::INT);
    stream.Write(5);
    stream.Write(10);
    EXPECT_EQ(output, "5 10");
}

TEST(WriteOnlyStreamTest, ToStringMultipleWrites) {
    std::string output;
    WriteOnlyStream<int> stream(output, DataType::INT);
    for (int i = 1; i <= 3; ++i) {
        stream.Write(i);
    }
    EXPECT_EQ(output, "1 2 3");
}

TEST(WriteOnlyStreamTest, StreamNotOpen) {
    std::string output;
    WriteOnlyStream<int> stream(output, DataType::INT);
    stream.Close();
    EXPECT_THROW(stream.Write(42), std::runtime_error);
}