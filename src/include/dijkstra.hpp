#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"
#include <optional>
#include <set>

class Dijkstra {
public:
    Dijkstra(Graph& g);

    [[nodiscard]] bool done();

    void marKShortestPaths();

private:
    void traverse(Graph::VertexType const& v);

    [[nodiscard]] bool completed() const;

    void setDestination();

    [[nodiscard]] Graph::VertexType extractFirst();

    Graph& graph;
    std::optional<Graph::VertexType> dst {};
    std::set<Graph::VertexType> unvisited {};
};

#endif