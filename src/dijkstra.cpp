#include "dijkstra.hpp"
#include <algorithm>

Dijkstra::Dijkstra(gr::Graph& g)
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
    if (completed())
        return true;

    auto current = extractFirst();
    if (!current.isStart())
        graph.markAs(current, gr::pointVisited);

    auto neighborhoods = graph.neighborhoods(current);
    for (auto nodeRef : neighborhoods) {
        auto& node = nodeRef.get();

        if (node.isStart())
            continue;
        else if (node.isEnd()) {
            dst = node;
            return true;
        }

        gr::Distance d = distance(current.pos(), node.pos());

        if (gr::Distance tentativeDist = current.dist() + d;
            tentativeDist < node.dist()) {
            if (!node.distIsInfinite()) {
                unvisited.erase(node);
                graph.markAs(node, gr::pointVisited);
            } else {
                graph.markAs(node, gr::pointFront);
            }
            node.setDist(tentativeDist);
            unvisited.insert(node);
        }
    }

    return false;
}

void Dijkstra::markShortestPaths()
{
    if (!dst.has_value())
        return;
    traverse(*dst);
}

void Dijkstra::traverse(gr::Graph::VertexType const& v)
{
    if (v.isStart())
        return;

    auto neigh = graph.neighborhoods(v);
    auto nearest = std::min_element(neigh.begin(), neigh.end(),
        [](gr::Graph::VertexType const& va, gr::Graph::VertexType const& vb) {
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
            graph.markAs(v, gr::pointBifurcation);
        graph.markAs(node, gr::pointShortest);
        traverse(node);
    }
}

bool Dijkstra::completed() const
{
    return unvisited.empty()
        || unvisited.begin()->distIsInfinite()
        || dst.has_value();
}

gr::Graph::VertexType Dijkstra::extractFirst()
{
    return unvisited.extract(*unvisited.begin()).value();
}
