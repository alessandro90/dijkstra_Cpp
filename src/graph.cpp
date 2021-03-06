#include "graph.hpp"
#include "io.hpp"
#include "optional_pointer.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <functional>
#include <iterator>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace gr {
Position operator+(Position const& a, Position const& b)
{
    return { { a.x + b.x }, { a.y + b.y } };
}
Position operator-(Position const& a, Position const& b)
{
    return { { a.x - b.x }, { a.y - b.y } };
}
std::ostream& operator<<(std::ostream& os, Position const& pos)
{
    return os << '{' << pos.x << ',' << pos.y << '}';
}
Distance distance(Position const& p1, Position const& p2)
{
    return Distance { std::sqrt(static_cast<double>(std::pow((p1.x - p2.x).value(), 2)
        + std::pow((p1.y - p2.y).value(), 2))) };
}

Vertex::Vertex(UniqueIdType id, CharType type_, Position const& p)
    : uniqueId { id }
    , mType { type_ }
    , mPos { p }
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

CharType Vertex::type() const { return mType; }

bool Vertex::isStart() const { return type() == pointStart; }

bool Vertex::isEnd() const { return type() == pointEnd; }

bool Vertex::isShortest() const
{
    return type() == pointShortest || type() == pointBifurcation;
}

bool Vertex::isValid(Vertex const& v, Graph& graph) const
{
    if (type() == pointObstacle)
        return false;

    if (auto const diff = v.pos() - pos(); diff.x.value() == 0 || diff.y.value() == 0)
        return true;

    auto const minXVertex = std::min(v, *this, [](Vertex const& a, Vertex const& b) {
        return a.pos().x < b.pos().x;
    });
    auto const minYVertex = std::min(v, *this, [](Vertex const& a, Vertex const& b) {
        return a.pos().y < b.pos().y;
    });

    auto const validDiagonal = [](OptionalPointer<Vertex> const& a, OptionalPointer<Vertex> const& b) {
        return a && b && a->type() != pointObstacle && b->type() != pointObstacle;
    };

    if (minXVertex == minYVertex) {
        OptionalPointer<Vertex> const a = graph.vertexPtr(minXVertex.pos() + Position { X { 1 }, Y { 0 } });
        OptionalPointer<Vertex> const b = graph.vertexPtr(minXVertex.pos() + Position { X { 0 }, Y { 1 } });
        return validDiagonal(a, b);
    }

    OptionalPointer<Vertex> const a = graph.vertexPtr(minXVertex.pos() + Position { X { 0 }, Y { -1 } });
    OptionalPointer<Vertex> const b = graph.vertexPtr(minXVertex.pos() + Position { X { 1 }, Y { 0 } });
    return validDiagonal(a, b);
}

Distance const& Vertex::dist() const { return mDist; }

bool Vertex::distIsInfinite() const { return dist() == infinite; }

void Vertex::setDist(Distance const& d) { mDist = d; }

void Vertex::setType(CharType t) { mType = t; }

Vertex::UniqueIdType Vertex::id() const { return uniqueId; }

Position const& Vertex::pos() const { return mPos; }

void Vertex::reset()
{
    if (type() != pointStart && type() != pointEnd) {
        setDist(infinite);
        setType(pointEmpty);
    }
}

void Graph::fromFile(std::string_view fname)
{
    io::File f { fname, io::in };
    VertexType::UniqueIdType elementCount { 0 };
    bool foundStart { false };
    bool foundEnd { false };
    auto lineCount { 0 };
    for (auto line : f) {
        auto colCount { 0 };
        std::vector<Vertex> v;
        v.reserve(line.size());
        for (auto c : line) {
            switch (static_cast<CharType>(c)) {
            case pointStart:
                if (foundStart)
                    throw InvalidGraphException {};
                foundStart = true;
                break;
            case pointEnd:
                if (foundEnd)
                    throw InvalidGraphException {};
                foundEnd = true;
                break;
            case pointEmpty:
            case pointObstacle:
                // Ok valid entries
                break;
            default:
                throw InvalidGraphException {};
                break;
            }
            v.emplace_back(elementCount, static_cast<CharType>(c), Position { X { lineCount }, Y { colCount } });
            colCount += 1;
            elementCount += 1;
        }
        vertex.push_back(std::move(v));
        lineCount += 1;
    }
}

void Graph::buildEmpty(unsigned sizeX, unsigned sizeY)
{
    vertex.resize(sizeX);
    int row {}, counter {};
    for (auto& line : vertex) {
        int col {};
        line.reserve(sizeY);
        for (unsigned i = 0; i < sizeY; ++i) {
            line.emplace_back(counter, pointEmpty, Position { X { row }, Y { col } });
            counter += 1;
            col += 1;
        }
        row += 1;
    }
    vertex[0][0].setType(pointStart);
    vertex[0][0].setDist(Distance { 0 });
    vertex[0][1].setType(pointEnd);
}

OptionalPointer<Graph::VertexType> Graph::vertexPtr(Position const& mPos)
{
    if (mPos.x.value() >= 0
        && mPos.y.value() >= 0
        && mPos.x.value() < static_cast<decltype(mPos.x.value())>(vertex.size())
        && mPos.y.value() < static_cast<decltype(mPos.y.value())>(vertex[mPos.x.value()].size()))
        return { &vertex[mPos.x.value()][mPos.y.value()] };
    return std::nullopt;
}

std::vector<std::reference_wrapper<Graph::VertexType>> Graph::neighborhoods(Graph::VertexType const& v)
{
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

    auto view = std::ranges::views::transform(coords, [&](Position const& pos) {
        return vertexPtr({ v.pos() - pos });
    }) | std::ranges::views::filter([&](OptionalPointer<Graph::VertexType> ptr) {
        return ptr && ptr->isValid(v, *this);
    }) | std::ranges::views::transform([](OptionalPointer<Graph::VertexType> ptr) {
        return std::ref(*ptr);
    });

    return { std::begin(view), std::end(view) };
}

std::vector<std::vector<Graph::VertexType>>& Graph::nodes() { return vertex; }
std::vector<std::vector<Graph::VertexType>> const& Graph::nodes() const { return vertex; }

void Graph::markAs(Graph::VertexType const& v, CharType pointType)
{
    auto pos = v.pos();
    vertex[pos.x.value()][pos.y.value()].setType(pointType);
}

std::string Graph::stringify() const
{
    std::string s {};
    for (auto const& line : vertex) {
        for (auto const& v : line) {
            std::ostringstream ss {};
            ss << v;
            s += ss.str();
        }
        s += '\n';
    }
    return s;
}

void Graph::reset()
{
    std::ranges::for_each(vertex | std::ranges::views::join, [](auto& item) { item.reset(); });
    maxDistance = Distance { 0 };
}

void Graph::updateMaxDistance(Distance newDisatnce)
{
    if (newDisatnce > maxDistance)
        maxDistance = newDisatnce;
}
Distance Graph::getMaxDistance() const
{
    return maxDistance;
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
}
