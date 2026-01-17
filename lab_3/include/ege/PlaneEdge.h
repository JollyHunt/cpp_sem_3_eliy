#ifndef PLANE_EDGE_H
#define PLANE_EDGE_H

#include "TransportEdge.h"

class PlaneEdge : public TransportEdge {
private:
    double airportDelay_;

public:
    PlaneEdge(const CommonVertex& from, const CommonVertex& to,
              double baseTime, double baseCost,
              double maxWeight, double maxSize,
              const std::string& weather,
              double airportDelay = 0.5)
        : TransportEdge(from, to, baseTime, baseCost, maxWeight, maxSize, weather),
          airportDelay_(airportDelay) {

        weightFunc_ = [this]() {
            return (baseTime_ + airportDelay_) * weatherFactor_;
        };
    }

    std::string getTypeName() const override { return "plane"; }
    TransportType getTransportType() const override { return TransportType::PLANE; }
};

#endif