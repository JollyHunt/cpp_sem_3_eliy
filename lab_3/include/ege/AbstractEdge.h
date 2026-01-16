#ifndef ABSTRACT_EDGE_H
#define ABSTRACT_EDGE_H

#include "../vertex/Vertex.h"
#include "WeatherCondition.h"
#include "TransportType.h"
#include <string>

class AbstractEdge {
protected:
    Vertex from_;
    Vertex to_;
    double baseTime_;
    double baseCost_;
    double maxWeight_;
    double maxSize_;
    double weatherFactor_;

public:
    AbstractEdge(const Vertex& from, const Vertex& to,
                 double baseTime, double baseCost,
                 double maxWeight, double maxSize,
                 const std::string& weatherStr)
        : from_(from), to_(to),
          baseTime_(baseTime), baseCost_(baseCost),
          maxWeight_(maxWeight), maxSize_(maxSize),
          weatherFactor_(weatherConditionToFactor(stringToWeather(weatherStr))) {}

    virtual ~AbstractEdge() = default;

    const Vertex& getFrom() const { return from_; }
    const Vertex& getTo() const { return to_; }
    double getMaxWeight() const { return maxWeight_; }
    double getMaxSize() const { return maxSize_; }

    virtual double getActualTime(double trafficFactor = 1.0) const = 0;
    virtual double getActualCost(double timeMultiplier = 1.0) const = 0;
    virtual std::string getTypeName() const = 0;
    virtual TransportType getTransportType() const = 0;

    bool canCarry(double weight, double size) const {
        return weight <= maxWeight_ && size <= maxSize_;
    }

    double getWeatherFactor() const { return weatherFactor_; }
};

#endif