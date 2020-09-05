#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"
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

    [[nodiscard]] Graph::VertexType extractNearest();

    Graph& graph;
    Graph::VertexType const* dst { nullptr };
    std::set<Graph::VertexType> unvisited {};
};

#endif