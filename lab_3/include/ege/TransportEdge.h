#ifndef TRANSPORT_EDGE_H
#define TRANSPORT_EDGE_H

#include "CommonEdge.h"
#include "WeatherCondition.h"
#include "TransportType.h"
#include <functional>
#include <string>

class TransportEdge : public CommonEdge {
protected:
    double baseTime_;
    double baseCost_;
    double maxWeight_;
    double maxSize_;
    double weatherFactor_;
    std::function<double()> weightFunc_; // <------------------------ 4:58 am idea ---------------------------- p.s. it sks but works

public:
    TransportEdge(const CommonVertex& from, const CommonVertex& to,
                  double baseTime, double baseCost,
                  double maxWeight, double maxSize,
                  const std::string& weatherStr)
        : CommonEdge(from, to),
          baseTime_(baseTime), baseCost_(baseCost),
          maxWeight_(maxWeight), maxSize_(maxSize),
          weatherFactor_(weatherConditionToFactor(stringToWeather(weatherStr))) {}

    double getWeight() const override {
        return weightFunc_();
    }

    bool canCarry(double weight, double size) const override {
        return weight <= maxWeight_ && size <= maxSize_;
    }

    double getMaxWeight() const { return maxWeight_; }
    double getMaxSize() const { return maxSize_; }

    double getActualTime() const {
        return weightFunc_();
    }

    double getActualCost() const {
        return baseCost_;
    }

    virtual std::string getTypeName() const = 0;
    virtual TransportType getTransportType() const = 0;
};

#endif