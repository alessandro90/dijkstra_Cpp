#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"
#include <optional>
#include <set>

class Dijkstra {
public:
    Dijkstra(gr::Graph& g);
    Dijkstra() = default;

    void loadGraph(gr::Graph& g);

    [[nodiscard]] bool done();

    void markShortestPaths();

private:
    void init(gr::Graph& g);
    void reset();

    void traverse(gr::Graph::VertexType const& v);

    [[nodiscard]] bool completed() const;

    [[nodiscard]] gr::Graph::VertexType extractFirst();

    gr::Graph* graph { nullptr };
    std::optional<gr::Graph::VertexType> dst {};
    std::set<gr::Graph::VertexType> unvisited {};
};

#endif