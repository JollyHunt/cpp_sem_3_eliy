#ifndef CAR_EDGE_H
#define CAR_EDGE_H

#include "TransportEdge.h"

class CarEdge : public TransportEdge {
public:
    CarEdge(const CommonVertex& from, const CommonVertex& to,
            double baseTime, double baseCost,
            double maxWeight, double maxSize,
            const std::string& weather,
            double trafficFactor = 1.0)
        : TransportEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather) {

        weightFunc_ = [this, trafficFactor]() {
            return baseTime_ * weatherFactor_ * trafficFactor;
        };
    }

    std::string getTypeName() const override { return "car"; }
    TransportType getTransportType() const override { return TransportType::CAR; }
};

#endif