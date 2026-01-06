#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "Stream.h"
#include <string>
#include <sstream>

template<typename T>
class Deserializer {
public:
    static T deserialize(const std::string& str, DataType type);
};

template<>
inline int Deserializer<int>::deserialize(const std::string& str, DataType type) {
    if (type != DataType::INT) {
        throw std::invalid_argument("Type mismatch");
    }
    return std::stoi(str);
}

template<>
inline std::string Deserializer<std::string>::deserialize(const std::string& str, DataType type) {
    if (type != DataType::STRING) {
        throw std::invalid_argument("Type mismatch");
    }
    return str;
}

template<>
inline char Deserializer<char>::deserialize(const std::string& str, DataType type) {
    if (type != DataType::CHAR) {
        throw std::invalid_argument("Type mismatch");
    }
    if (str.empty()) throw std::invalid_argument("Empty char");
    return str[0];
}

#endif