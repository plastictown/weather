#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include "weather.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace pt = boost::property_tree;
using tcp = net::ip::tcp;

namespace weather
{
    const std::string GetWeather(const std::string& host, const short port, const std::string& uri, const std::string& token,
        const int httpVersion)
    {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);
        auto const results = resolver.resolve(host, std::to_string(port));
        stream.connect(results);
        http::request<http::string_body> req{ http::verb::get, uri + "&appid=" + token, httpVersion };
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        if (ec && ec != beast::errc::not_connected)
            throw beast::system_error{ ec };

        return std::string{ static_cast<const char*>((*res.body().data().begin()).data()), (*res.body().data().begin()).size() };
    }

    const struct Weather WeatherFromJson(const std::string& data)
    {
        pt::ptree resp;
        std::stringstream ss;
        ss << data;
        struct Weather w{};
        pt::read_json(ss, resp);
        const auto& mainChild = resp.get_child("main");
        w.tempMin = mainChild.get_child("temp_min").get_value<decltype(w.tempMin)>();
        w.tempMax = mainChild.get_child("temp_max").get_value<decltype(w.tempMax)>();
        w.feelsLike = mainChild.get_child("feels_like").get_value<decltype(w.feelsLike)>();
        w.pressure = mainChild.get_child("pressure").get_value<decltype(w.pressure)>();
        w.wind = resp.get_child("wind.speed").get_value<decltype(w.wind)>();
        return w;
    }

    std::ostream& operator << (std::ostream& o, const struct Weather& w)
    {
        o << "min. temp: " << w.FromAbs(w.tempMin) << " C" << std::endl;
        o << "max. temp: " << w.FromAbs(w.tempMax) << " C" << std::endl;
        o << "feels like temp: " << w.FromAbs(w.feelsLike) << " C" << std::endl;
        o << "wind speed: " << w.wind << " m/s" << std::endl;
        o << "pressure: " << w.FromHPa(w.pressure) << " mmHg" << std::endl;
        return o;
    }
} // namespace weather