#ifndef GRAPH_H
#define GRAPH_H

#include "number.hpp"
#include <compare>
#include <functional>
#include <limits>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>

using X = num::Number<int, struct TypeX>;
using Y = num::Number<int, struct TypeY>;
using Distance = num::Number<float, struct Dist>;

struct Position {
    X x;
    Y y;
};

Position operator+(Position const& a, Position const& b);
Position operator-(Position const& a, Position const& b);
bool operator==(Position const& a, Position const& b);
bool operator!=(Position const& a, Position const& b);
std::ostream& operator<<(std::ostream& os, Position const& pos);
Distance distance(Position const& p1, Position const& p2);

inline constexpr char pointEmpty = '*';
inline constexpr char pointObstacle = 'X';
inline constexpr char pointShortest = 'S';
inline constexpr char pointVisited = 'V';
inline constexpr char pointStart = 'A';
inline constexpr char pointEnd = 'B';
inline constexpr Distance infinite { std::numeric_limits<typename Distance::value_type>::max() };

struct Vertex {
public:
    using UniqueIdType = int;
    explicit Vertex(char type_, Position const& p, UniqueIdType id);
    friend std::ostream& operator<<(std::ostream& os, Vertex const& v);
    [[nodiscard]] Position const& getPos() const;
    void setDist(Distance const& d);
    void setType(char t);
    [[nodiscard]] bool isStart() const;
    [[nodiscard]] bool isEnd() const;
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] char type() const;
    [[nodiscard]] Distance const& dist() const;
    [[nodiscard]] bool distIsInfinite() const;
    [[nodiscard]] auto operator<=>(Vertex const& v) const -> std::common_comparison_category_t<decltype(std::declval<UniqueIdType>() <=> std::declval<UniqueIdType>()), decltype(std::declval<Vertex>().dist() <=> std::declval<Vertex>().dist())>;

private:
    char mType;
    Position pos;
    Distance mDist { infinite };
    // Needed to insert vertex in a set
    UniqueIdType const uniqueId;
};

class Graph {
public:
    using VertexType = Vertex;
    inline static constexpr unsigned closests { 8 };
    explicit Graph(std::string_view fname);

    [[nodiscard]] VertexType* getVertexPtr(Position const& pos);
    [[nodiscard]] std::vector<std::reference_wrapper<VertexType>> getNeighborhoods(VertexType const& v);
    [[nodiscard]] std::string stringify() const;
    [[nodiscard]] std::vector<std::vector<VertexType>>& getNodes();
    void markAsShortest(VertexType const& v);
    void markAsVisited(VertexType const& v);

private:
    std::vector<std::vector<VertexType>> vertex {};
};

std::ostream& operator<<(std::ostream& os, Graph const& lvl);
std::ostream& operator<<(std::ostream& os, Graph::VertexType const& v);
void writeGraph(std::string_view fname, Graph const& graph);

#endif
