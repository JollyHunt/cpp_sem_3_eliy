#ifndef TRUCK_EDGE_H
#define TRUCK_EDGE_H

#include "TransportEdge.h"

class TruckEdge : public TransportEdge {
public:
    TruckEdge(const CommonVertex& from, const CommonVertex& to,
              double baseTime, double baseCost,
              double maxWeight, double maxSize,
              const std::string& weather,
              double trafficFactor = 1.0)
        : TransportEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather) {

        weightFunc_ = [this, trafficFactor]() {
            return baseTime_ * weatherFactor_ * trafficFactor;
        };
    }

    std::string getTypeName() const override { return "truck"; }
    TransportType getTransportType() const override { return TransportType::TRUCK; }
};

#endif