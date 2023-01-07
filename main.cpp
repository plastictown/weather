#include <cstdlib>
#include <iostream>
#include <exception>
#include "weather.h"

int main(int argc, char** argv)
{
    constexpr auto host = "api.openweathermap.org";
    constexpr short port = 80;
    constexpr auto target = "/data/2.5/weather?lat=*&lon=*";
    constexpr auto token = "*";

    try
    {
        std::cout << weather::WeatherFromJson(weather::GetWeather(host, port, target, token)) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cin.get();
    return EXIT_SUCCESS;
}