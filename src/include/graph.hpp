#ifndef GRAPH_H
#define GRAPH_H

#include "number.hpp"
#include "optional_pointer.hpp"
#include <compare>
#include <exception>
#include <functional>
#include <limits>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>

namespace gr {

using X = num::Number<int, struct TypeX>;
using Y = num::Number<int, struct TypeY>;
using Distance = num::Number<double, struct Dist>;
enum CharType : unsigned char {};

struct Position {
    X x;
    Y y;
    auto operator<=>(Position const&) const = default;
};

Position operator+(Position const& a, Position const& b);
Position operator-(Position const& a, Position const& b);
std::ostream& operator<<(std::ostream& os, Position const& pos);
Distance distance(Position const& p1, Position const& p2);

inline constexpr CharType pointEmpty { '*' };
inline constexpr CharType pointObstacle { 'X' };
inline constexpr CharType pointShortest { 254 };
inline constexpr CharType pointBifurcation { 219 };
// inline constexpr CharType pointShortest { 254u };
inline constexpr CharType pointVisited { '.' };
inline constexpr CharType pointFront { 'f' };
inline constexpr CharType pointStart { 'A' };
inline constexpr CharType pointEnd { 'B' };
inline constexpr Distance infinite { std::numeric_limits<typename Distance::value_type>::max() };

class Graph;

struct Vertex {
public:
    using UniqueIdType = int;
    explicit Vertex(UniqueIdType id, CharType type_, Position const& p);
    friend std::ostream& operator<<(std::ostream& os, Vertex const& v);
    [[nodiscard]] Position const& pos() const;
    void setDist(Distance const& d);
    void setType(CharType t);
    [[nodiscard]] bool isStart() const;
    [[nodiscard]] bool isEnd() const;
    [[nodiscard]] bool isShortest() const;
    [[nodiscard]] bool isValid(Vertex const& v, Graph& graph) const;
    [[nodiscard]] CharType type() const;
    [[nodiscard]] Distance const& dist() const;
    [[nodiscard]] bool distIsInfinite() const;
    [[nodiscard]] auto operator<=>(Vertex const& v) const = default;
    [[nodiscard]] UniqueIdType id() const;
    void reset();

private:
    Distance mDist { infinite };
    // Needed to insert vertex in a set
    UniqueIdType uniqueId {};
    CharType mType {};
    Position mPos {};
};

class InvalidGraphException : std::exception {
public:
    const char* what() const noexcept override
    {
        return "Invalid graph point type";
    }
};

class Graph {
public:
    using VertexType = Vertex;
    inline static constexpr unsigned closests { 8 };

    [[nodiscard]] OptionalPointer<VertexType> vertexPtr(Position const& pos);
    [[nodiscard]] std::vector<std::reference_wrapper<VertexType>> neighborhoods(VertexType const& v);
    [[nodiscard]] std::string stringify() const;
    [[nodiscard]] std::vector<std::vector<VertexType>>& nodes();
    [[nodiscard]] std::vector<std::vector<VertexType>> const& nodes() const;
    void markAs(VertexType const& v, CharType);
    void fromFile(std::string_view fname);
    void buildEmpty(unsigned sizeX, unsigned sizeY);
    void reset();
    void updateMaxDistance(Distance newDistance);
    Distance getMaxDistance() const;

private:
    std::vector<std::vector<VertexType>> vertex {};
    Distance maxDistance {};
};

std::ostream& operator<<(std::ostream& os, Graph const& lvl);
std::ostream& operator<<(std::ostream& os, Graph::VertexType const& v);
void writeGraph(std::string_view fname, Graph const& graph);
}
#endif
