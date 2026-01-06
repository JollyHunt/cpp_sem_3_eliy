#ifndef STREAM_H
#define STREAM_H

#include <string>
#include <stdexcept>

enum class DataType {
    INT,
    STRING,
    CHAR
};

class EndOfStreamException : public std::runtime_error {
public:
    EndOfStreamException() : std::runtime_error("End of stream reached") {}
};

#endif