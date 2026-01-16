#ifndef TRANSPORT_TYPE_H
#define TRANSPORT_TYPE_H

#include <string>
#include <stdexcept>

enum class TransportType {
    SHIP,
    TRUCK,
    PLANE,
    CAR
};

inline std::string transportToString(TransportType tt) {
    switch (tt) {
        case TransportType::SHIP: return "ship";
        case TransportType::TRUCK: return "truck";
        case TransportType::PLANE: return "plane";
        case TransportType::CAR: return "car";
        default: return "unknown";
    }
}

inline TransportType stringToTransport(const std::string& s) {
    if (s == "ship") return TransportType::SHIP;
    if (s == "truck") return TransportType::TRUCK;
    if (s == "plane") return TransportType::PLANE;
    if (s == "car") return TransportType::CAR;
    throw std::invalid_argument("Unknown transport type");
}

#endif