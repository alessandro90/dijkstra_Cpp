#include "dijkstra.hpp"
#include <algorithm>

Dijkstra::Dijkstra(Graph& g)
    : graph { g }
{
    auto& vertices = graph.nodes();
    for (auto& row : vertices) {
        if (auto it = std::find_if(
                row.begin(),
                row.end(),
                [](auto& node) { return node.isStart(); });
            it != row.end()) {
            unvisited.insert(*it);
            break;
        }
    }
}

bool Dijkstra::done()
{
    if (completed()) {
        setDestination();
        return true;
    }

    auto current = extractFirst();

    auto neighborhoods = graph.neighborhoods(current);
    for (auto nodeRef : neighborhoods) {
        auto& node = nodeRef.get();
        if (!node.isStart() && !node.isEnd())
            graph.markAsVisited(node);

        Distance d = distance(current.pos(), node.pos());

        if (Distance tentativeDist = current.dist() + d;
            tentativeDist < node.dist()) {
            if (!node.distIsInfinite())
                unvisited.erase(node);
            node.setDist(tentativeDist);
            unvisited.insert(node);
        }
    }

    return false;
}

void Dijkstra::marKShortestPaths()
{
    if (!dst.has_value())
        return;
    traverse(*dst);
}

void Dijkstra::traverse(Graph::VertexType const& v)
{
    if (v.isStart())
        return;

    auto neigh = graph.neighborhoods(v);
    auto nearest = std::min_element(neigh.begin(), neigh.end(),
        [](Graph::VertexType const& va, Graph::VertexType const& vb) {
            return va.dist() < vb.dist();
        });

    for (auto& n : neigh) {
        auto& node = n.get();
        if (node.dist() > nearest->get().dist()
            || node.isStart()
            || node.isEnd()
            || node.isShortest()) {
            continue;
        }
        if (nearest->get().pos() != node.pos())
            graph.markAsBifurcation(v);
        graph.markAsShortest(node);
        traverse(node);
    }
}

bool Dijkstra::completed() const
{
    return unvisited.empty()
        || unvisited.begin()->distIsInfinite()
        || unvisited.begin()->isEnd();
}

void Dijkstra::setDestination()
{
    if (!unvisited.empty() && unvisited.begin()->isEnd())
        dst = extractFirst();
}

Graph::VertexType Dijkstra::extractFirst()
{
    return unvisited.extract(*unvisited.begin()).value();
}