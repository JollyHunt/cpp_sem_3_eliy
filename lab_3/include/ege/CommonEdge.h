#ifndef COMMON_EDGE_H
#define COMMON_EDGE_H

#include "../vertex/CommonVertex.h"

class CommonEdge {
protected:
    CommonVertex from_;
    CommonVertex to_;

public:
    CommonEdge(const CommonVertex& from, const CommonVertex& to)
        : from_(from), to_(to) {}

    virtual ~CommonEdge() = default;

    const CommonVertex& getFrom() const { return from_; }
    const CommonVertex& getTo() const { return to_; }

    virtual double getWeight() const = 0;

    virtual bool canCarry(double weight, double size) const { return true; }
};

#endif