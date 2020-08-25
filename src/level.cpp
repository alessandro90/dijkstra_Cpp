#include "include/level.hpp"
#include "include/io.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <utility>

namespace lv {

Level::Level(std::string_view fname)
{
    io::File f { fname, io::in };
    for (auto line : f) {
        std::vector<Vertex> v;
        v.reserve(line.size());
        std::transform(line.begin(), line.end(), std::back_inserter(v), [](const char c) {
            return Vertex { c };
        });
        vertex.push_back(std::move(v));
    }
}

Vertex& Level::operator()(Position pos)
{
    if (pos.x.value() < vertex.size() && pos.y.value() < vertex[0].size())
        return vertex[pos.x.value()][pos.y.value()];
    return sentinel;
}

std::string Level::stringify() const
{
    std::string s {};
    for (auto const& line : vertex) {
        std::transform(line.begin(), line.end(), std::back_inserter(s), [](Vertex const& v) {
            return v.type();
        });
        s += '\n';
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, Level const& lvl)
{
    return os << lvl.stringify();
}

std::ostream& operator<<(std::ostream& os, lv::Vertex const& v)
{
    os << '{' << v.type() << ':' << v.dist() << '}';
    return os;
}

void writeLevel(std::string_view fname, Level const& level)
{
    io::File { fname, io::out }.write(level.stringify().c_str());
}

}
