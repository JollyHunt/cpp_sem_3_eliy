#ifndef CAR_EDGE_H
#define CAR_EDGE_H

#include "AbstractEdge.h"

class CarEdge : public AbstractEdge {
public:
    CarEdge(const Vertex& from, const Vertex& to,
            double baseTime, double baseCost,
            double maxWeight, double maxSize,
            const std::string& weather)
        : AbstractEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather) {}

    double getActualTime(double trafficFactor = 1.0) const override {
        return baseTime_ * weatherFactor_ * trafficFactor;
    }

    double getActualCost(double timeMultiplier = 1.0) const override {
        return baseCost_ * timeMultiplier;
    }

    std::string getTypeName() const override { return "car"; }
    TransportType getTransportType() const override { return TransportType::CAR; }
};

#endif