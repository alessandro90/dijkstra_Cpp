#include "graph.hpp"
#include "io.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

Position operator+(Position const& a, Position const& b)
{
    return { { a.x + b.x }, { a.y + b.y } };
}
Position operator-(Position const& a, Position const& b)
{
    return { { a.x - b.x }, { a.y - b.y } };
}
bool operator==(Position const& a, Position const& b)
{
    return a.x == b.x && a.y == b.y;
}
bool operator!=(Position const& a, Position const& b)
{
    return !(a == b);
}
std::ostream& operator<<(std::ostream& os, Position const& pos)
{
    return os << '{' << pos.x << ',' << pos.y << '}';
}
Distance distance(Position const& p1, Position const& p2)
{
    return Distance { std::sqrt(static_cast<float>(std::pow((p1.x - p2.x).value(), 2)
        + std::pow((p1.y - p2.y).value(), 2))) };
}

Vertex::Vertex(char type_, Position const& p, UniqueIdType id)
    : mType { type_ }
    , pos { p }
    , uniqueId { id }
{
    switch (mType) {
    case pointEmpty:
    case pointEnd:
    case pointObstacle:
        setDist(infinite);
        break;
    case pointStart:
        setDist(Distance { 0.f });
        break;
    }
}

char Vertex::type() const { return mType; }

bool Vertex::isStart() const { return type() == pointStart; }

bool Vertex::isEnd() const { return type() == pointEnd; }

bool Vertex::isValid() const { return type() != pointObstacle; }

Distance const& Vertex::dist() const
{
    return mDist;
}

bool Vertex::distIsInfinite() const { return dist() == infinite; }

void Vertex::setDist(Distance const& d) { mDist = d; }

void Vertex::setType(char t) { mType = t; }

auto Vertex::operator<=>(Vertex const& v) const -> std::common_comparison_category_t<decltype(std::declval<UniqueIdType>() <=> std::declval<UniqueIdType>()), decltype(std::declval<Vertex>().dist() <=> std::declval<Vertex>().dist())>
{
    if (dist() != v.dist())
        return dist() <=> v.dist();
    return uniqueId <=> v.uniqueId;
}

Position const& Vertex::getPos() const
{
    return pos;
}

Graph::Graph(std::string_view fname)
{
    io::File f { fname, io::in };
    VertexType::UniqueIdType elementCount { 0 };
    auto lineCount { 0 };
    for (auto line : f) {
        auto colCount { 0 };
        std::vector<Vertex> v;
        v.reserve(line.size());
        for (auto c : line) {
            v.emplace_back(c, Position { X { lineCount }, Y { colCount } }, elementCount);
            colCount += 1;
            elementCount += 1;
        }
        vertex.push_back(std::move(v));
        lineCount += 1;
    }
}

Graph::VertexType* Graph::getVertexPtr(Position const& pos)
{
    if (pos.x.value() < static_cast<int>(vertex.size())
        && pos.y.value() < static_cast<int>(vertex[0].size())
        && pos.x.value() >= 0
        && pos.y.value() >= 0)
        return &vertex[pos.x.value()][pos.y.value()];
    return nullptr;
}

std::vector<std::reference_wrapper<Graph::VertexType>> Graph::getNeighborhoods(Graph::VertexType const& v)
{
    std::vector<std::reference_wrapper<Vertex>> neighborhoods;
    neighborhoods.reserve(Graph::closests);

    constexpr static std::array coords {
        Position { X { -1 }, Y { -1 } },
        Position { X { 0 }, Y { -1 } },
        Position { X { 1 }, Y { -1 } },
        Position { X { -1 }, Y { 0 } },
        Position { X { 1 }, Y { 0 } },
        Position { X { -1 }, Y { 1 } },
        Position { X { 0 }, Y { 1 } },
        Position { X { 1 }, Y { 1 } }
    };

    for (auto const& pos : coords) {
        if (auto ptr = getVertexPtr({ v.getPos() - pos });
            ptr != nullptr && ptr->isValid())
            neighborhoods.push_back(*ptr);
    }
    return neighborhoods;
}

std::vector<std::vector<Graph::VertexType>>& Graph::getNodes() { return vertex; }

void Graph::markAsShortest(Graph::VertexType const& v)
{
    auto pos = v.getPos();
    vertex[pos.x.value()][pos.y.value()].setType(pointShortest);
}
void Graph::markAsVisited(Graph::VertexType const& v)
{
    auto pos = v.getPos();
    vertex[pos.x.value()][pos.y.value()].setType(pointVisited);
}

std::string Graph::stringify() const
{
    std::string s {};
    for (auto const& line : vertex) {
        for (auto v : line) {
            std::ostringstream ss {};
            ss << v;
            s += ss.str();
        }
        s += '\n';
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, Graph const& lvl)
{
    return os << lvl.stringify();
}

std::ostream& operator<<(std::ostream& os, Graph::VertexType const& v)
{
    return os << v.type();
}

void writeGraph(std::string_view fname, Graph const& graph)
{
    io::File { fname, io::out }.write(graph.stringify().c_str());
}
