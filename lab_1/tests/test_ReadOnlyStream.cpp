#include <gtest/gtest.h>
#include "stream/ReadOnlyStream.h"
#include "sequence/Sequence.h"
#include <string>

TEST(ReadOnlyStreamTest, FromStringInt) {
    std::string data = "10 20 30";
    ReadOnlyStream<int> stream(data, DataType::INT);
    EXPECT_EQ(stream.Read(), 10);
    EXPECT_EQ(stream.Read(), 20);
    EXPECT_EQ(stream.Read(), 30);
    EXPECT_TRUE(stream.IsEndOfStream());
}

TEST(ReadOnlyStreamTest, FromSequence) {
    ListSequence<int> seq;
    seq.Append(100);
    seq.Append(200);
    ReadOnlyStream<int> stream(&seq, DataType::INT);
    EXPECT_EQ(stream.Read(), 100);
    EXPECT_EQ(stream.Read(), 200);
    EXPECT_TRUE(stream.IsEndOfStream());
}

TEST(ReadOnlyStreamTest, SeekInSequence) {
    ListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    ReadOnlyStream<int> stream(&seq, DataType::INT);
    stream.Read();
    stream.Seek(0);
    EXPECT_EQ(stream.Read(), 1);
}

TEST(ReadOnlyStreamTest, EndOfStreamException) {
    std::string data = "5";
    ReadOnlyStream<int> stream(data, DataType::INT);
    stream.Read();
    EXPECT_THROW(stream.Read(), EndOfStreamException);
}