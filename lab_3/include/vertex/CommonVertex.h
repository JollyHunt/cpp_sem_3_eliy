#ifndef COMMON_VERTEX_H
#define COMMON_VERTEX_H

#include <string>

class CommonVertex {
private:
    std::string id_;

public:
    CommonVertex() = default;
    explicit CommonVertex(const std::string& id) : id_(id) {}

    const std::string& getId() const { return id_; }

    bool operator==(const CommonVertex& other) const {
        return id_ == other.id_;
    }

    bool operator<(const CommonVertex& other) const {
        return id_ < other.id_;
    }
};

#endif