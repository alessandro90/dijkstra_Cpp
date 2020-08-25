#ifndef LEVEL_H
#define LEVEL_H

#include <compare>
#include <cstddef>
#include <ostream>
#include <string_view>
#include <vector>

namespace lv {

template <typename Tag>
struct Coordinate {
public:
    explicit Coordinate(std::size_t c)
        : value_ { c }
    {
    }
    std::size_t value() const { return value_; }

private:
    std::size_t value_ {};
};

using X = Coordinate<struct TypeX>;
using Y = Coordinate<struct TypeY>;

struct Position {
    X x;
    Y y;
};

struct Vertex {
public:
    static constexpr int unvisited { -1 };
    static constexpr char sentinelType { 'X' };
    explicit Vertex(char type_)
        : mType { type_ }
    {
    }

    char type() const { return mType; }
    int dist() const { return mDist; }
    bool setDist(int d)
    {
        if (!sentinel()) {
            mDist = d;
            return true;
        }
        return false;
    }
    bool visited() const { return dist() == unvisited; }
    bool sentinel() const { return type() == sentinelType; }
    auto operator<=>(Vertex const& v) const
    {
        using ordering = decltype(dist() <=> v.dist());
        if (sentinel() && v.sentinel())
            return ordering::equivalent;
        else if (sentinel())
            return ordering::greater;
        else if (v.sentinel())
            return ordering::less;
        return dist() <=> v.dist();
    }
    auto operator==(Vertex const& v) const
    {
        if ((!sentinel() && !v.sentinel()) || (sentinel() && v.sentinel()))
            return dist() == v.dist();
        return false;
    }

private:
    char mType;
    int mDist { unvisited };
};

class Level {
public:
    explicit Level(std::string_view fname);

    Vertex& operator()(Position pos);
    std::string stringify() const;

    static constexpr char pointEmpty = '*';
    static constexpr char pointFull = 'X';
    static constexpr char pointStart = 'A';
    static constexpr char pointEnd = 'B';

private:
    std::vector<std::vector<Vertex>> vertex {};
    Vertex sentinel { Vertex::sentinelType };
};

std::ostream& operator<<(std::ostream& os, Level const& lvl);
std::ostream& operator<<(std::ostream& os, lv::Vertex const& v);
void writeLevel(std::string_view fname, Level const& level);

}
#endif
