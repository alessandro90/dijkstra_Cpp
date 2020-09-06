#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"
#include <optional>
#include <set>

class Dijkstra {
public:
    Dijkstra(gr::Graph& g);

    [[nodiscard]] bool done();

    void marKShortestPaths();

private:
    void traverse(gr::Graph::VertexType const& v);

    [[nodiscard]] bool completed() const;

    void setDestination();

    [[nodiscard]] gr::Graph::VertexType extractFirst();

    gr::Graph& graph;
    std::optional<gr::Graph::VertexType> dst {};
    std::set<gr::Graph::VertexType> unvisited {};
};

#endif