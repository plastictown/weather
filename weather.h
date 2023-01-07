#pragma once
#ifndef __WEATHER_H__
#define __WEATHER_H__

#include <string>
#include <ostream>

namespace weather
{
    struct Weather
    {
        float tempMin;
        float tempMax;
        float pressure;
        float feelsLike;
        float wind;

        static float FromAbs(float t)
        {
            return absZero + t;
        }
        static float FromHPa(float p)
        {
            return p / coefHpaToMmHg; // mmHg
        }
    private:
        static constexpr float coefHpaToMmHg = 1.333f;
        static constexpr float absZero = -273.15f;
    };

    std::ostream& operator << (std::ostream& o, const struct Weather& w);
    const std::string GetWeather(const std::string& host, const short port, const std::string& uri, const std::string& token,
        const int httpVersion = 10);
    const struct Weather WeatherFromJson(const std::string& data);
} // namespace weather

#endif // !__WEATHER_H__
