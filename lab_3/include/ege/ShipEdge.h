#ifndef SHIP_EDGE_H
#define SHIP_EDGE_H

#include "AbstractEdge.h"

class ShipEdge : public AbstractEdge {
public:
    ShipEdge(const Vertex& from, const Vertex& to,
             double baseTime, double baseCost,
             double maxWeight, double maxSize,
             const std::string& weather)
        : AbstractEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather) {}

    double getActualTime(double = 1.0) const override {
        return baseTime_ * weatherFactor_;
    }

    double getActualCost(double = 1.0) const override {
        return baseCost_;
    }

    std::string getTypeName() const override { return "ship"; }
    TransportType getTransportType() const override { return TransportType::SHIP; }
};

#endif