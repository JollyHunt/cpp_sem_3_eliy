#ifndef VERTEX_H
#define VERTEX_H

#include <string>

class Vertex {
private:
    std::string name_;
    double transferTime_;
    double transferCost_;

public:
    Vertex() : name_(""), transferTime_(0.0), transferCost_(0.0) {}

    Vertex(const std::string& name, double transferTime = 0.0, double transferCost = 0.0)
        : name_(name), transferTime_(transferTime), transferCost_(transferCost) {}

    const std::string& getName() const { return name_; }
    double getTransferTime() const { return transferTime_; }
    double getTransferCost() const { return transferCost_; }

    bool operator==(const Vertex& other) const {
        return name_ == other.name_;
    }

    bool operator<(const Vertex& other) const {
        return name_ < other.name_;
    }
};

#endif