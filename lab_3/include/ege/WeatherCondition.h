#ifndef WEATHER_CONDITION_H
#define WEATHER_CONDITION_H

#include <string>

enum class WeatherCondition {
    CLEAR,
    LIGHT_RAIN,
    HEAVY_RAIN,
    STORM,
    FOG,
    SNOW
};

inline double weatherConditionToFactor(WeatherCondition wc) {
    switch (wc) {
        case WeatherCondition::CLEAR: return 1.0;
        case WeatherCondition::LIGHT_RAIN: return 1.2;
        case WeatherCondition::HEAVY_RAIN: return 1.5;
        case WeatherCondition::STORM: return 2.0;
        case WeatherCondition::FOG: return 1.75;
        case WeatherCondition::SNOW: return 1.75;
        default: return 1.0;
    }
}

inline std::string weatherToString(WeatherCondition wc) {
    switch (wc) {
        case WeatherCondition::CLEAR: return "clear";
        case WeatherCondition::LIGHT_RAIN: return "light_rain";
        case WeatherCondition::HEAVY_RAIN: return "heavy_rain";
        case WeatherCondition::STORM: return "storm";
        case WeatherCondition::FOG: return "fog";
        case WeatherCondition::SNOW: return "snow";
        default: return "unknown";
    }
}

inline WeatherCondition stringToWeather(const std::string& s) {
    if (s == "clear") return WeatherCondition::CLEAR;
    if (s == "light_rain") return WeatherCondition::LIGHT_RAIN;
    if (s == "heavy_rain") return WeatherCondition::HEAVY_RAIN;
    if (s == "storm") return WeatherCondition::STORM;
    if (s == "fog") return WeatherCondition::FOG;
    if (s == "snow") return WeatherCondition::SNOW;
    return WeatherCondition::CLEAR;
}

#endif