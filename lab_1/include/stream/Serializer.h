#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Stream.h"
#include <string>

template<typename T>
class Serializer {
public:
    static std::string serialize(const T& value, DataType type);
};

template<>
inline std::string Serializer<int>::serialize(const int& value, DataType type) {
    if (type != DataType::INT) {
        throw std::invalid_argument("Type mismatch");
    }
    return std::to_string(value);
}

template<>
inline std::string Serializer<std::string>::serialize(const std::string& value, DataType type) {
    if (type != DataType::STRING) {
        throw std::invalid_argument("Type mismatch");
    }
    return value;
}

template<>
inline std::string Serializer<char>::serialize(const char& value, DataType type) {
    if (type != DataType::CHAR) {
        throw std::invalid_argument("Type mismatch");
    }
    return std::string(1, value);
}

#endif