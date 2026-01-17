#ifndef SHIP_EDGE_H
#define SHIP_EDGE_H

#include "TransportEdge.h"

class ShipEdge : public TransportEdge {
public:
    ShipEdge(const CommonVertex& from, const CommonVertex& to,
             double baseTime, double baseCost,
             double maxWeight, double maxSize,
             const std::string& weather)
        : TransportEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather) {

        weightFunc_ = [this]() {
            return baseTime_ * weatherFactor_;
        };
    }

    std::string getTypeName() const override { return "ship"; }
    TransportType getTransportType() const override { return TransportType::SHIP; }
};

#endif