#include "dijkstra.hpp"
#include <algorithm>

Dijkstra::Dijkstra(Graph& g)
    : graph { g }
{
    auto& vertices = graph.getNodes();
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

    auto current = extractNearest();

    auto neighborhoods = graph.getNeighborhoods(current);
    for (auto nodeRef : neighborhoods) {
        auto& node = nodeRef.get();
        if (!node.isStart() && !node.isEnd())
            graph.markAsVisited(node);

        Distance d = distance(current.getPos(), node.getPos());

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
    if (dst == nullptr)
        return;

    traverse(*dst);
}

void Dijkstra::traverse(Graph::VertexType const& v)
{
    if (v.isStart())
        return;
    auto neigh = graph.getNeighborhoods(v);
    auto nearest = std::min_element(neigh.begin(), neigh.end(), [](Graph::VertexType const& va, Graph::VertexType const& vb) {
        return va.dist() < vb.dist();
    });
    for (auto& n : neigh) {
        if (n.get().dist() != nearest->get().dist() || n.get().isStart())
            continue;
        graph.markAsShortest(n.get());
        traverse(n.get());
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
        dst = std::addressof(*unvisited.begin());
}

Graph::VertexType Dijkstra::extractNearest()
{
    return unvisited.extract(*unvisited.begin()).value();
}