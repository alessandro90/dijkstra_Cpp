#include "io.hpp"
#include <algorithm>
#include <cctype>
#include <unordered_map>

class ConfigParser {
public:
    ConfigParser() = default;
    void parse()
    {
        io::File file { "../text_files/config.txt" };
        for (auto const& line : file) {
            std::string ln { line };
            ln.erase(
                std::remove_if(ln.begin(), ln.end(),
                    [](auto const& c) { return std::isspace(c); }),
                ln.end());
            auto pos = ln.find(':');
            config[ln.substr(0, pos)] = ln.substr(pos + 1);
        }
    }

    std::string_view get(std::string_view property) const
    {
        return config.at(property.data());
    }

private:
    std::unordered_map<std::string, std::string> config {};
};