#ifndef PLANE_EDGE_H
#define PLANE_EDGE_H

#include "AbstractEdge.h"

class PlaneEdge : public AbstractEdge {
private:
    double airportDelay_;

public:
    PlaneEdge(const Vertex& from, const Vertex& to,
              double baseTime, double baseCost,
              double maxWeight, double maxSize,
              const std::string& weather,
              double airportDelay = 0.5)
        : AbstractEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather),
          airportDelay_(airportDelay) {}

    double getActualTime(double = 1.0) const override {
        return (baseTime_ + airportDelay_) * weatherFactor_;
    }

    double getActualCost(double = 1.0) const override {
        return baseCost_;
    }

    std::string getTypeName() const override { return "plane"; }
    TransportType getTransportType() const override { return TransportType::PLANE; }
};

#endif