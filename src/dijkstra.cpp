#include "dijkstra.hpp"
#include <algorithm>
#include <optional>
#include <ranges>

Dijkstra::Dijkstra(gr::Graph& g)
{
    init(g);
}

void Dijkstra::loadGraph(gr::Graph& g)
{
    init(g);
}

void Dijkstra::reset()
{
    dst = std::nullopt;
    unvisited.clear();
    graph = nullptr;
}

void Dijkstra::init(gr::Graph& g)
{
    reset();
    graph = &g;
    auto& vertices = graph->nodes();
    auto joinView = vertices | std::ranges::views::join;
    auto it = std::ranges::find_if(joinView, [](auto const& node) {
        return node.isStart();
    });
    if (it != std::end(joinView))
        unvisited.insert(*it);
}

bool Dijkstra::done()
{
    if (completed())
        return true;

    auto current = extractFirst();
    if (!current.isStart())
        graph->markAs(current, gr::pointVisited);

    auto neighborhoods = graph->neighborhoods(current);
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
                graph->markAs(node, gr::pointVisited);
            } else {
                graph->markAs(node, gr::pointFront);
            }
            node.setDist(tentativeDist);
            graph->updateMaxDistance(tentativeDist);
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

    auto neigh = graph->neighborhoods(v);
    auto nearest = std::min_element(neigh.begin(), neigh.end(),
        [](gr::Graph::VertexType const& va, gr::Graph::VertexType const& vb) {
            return va.dist() < vb.dist();
        });

    std::ranges::for_each(neigh | std::ranges::views::filter([&](auto const& node) {
        return !(node.get().dist() > nearest->get().dist()
            || node.get().isStart()
            || node.get().isEnd()
            || node.get().isShortest());
    }),
        [&](auto& node) {
            if (nearest->get().pos() != node.get().pos() && !v.isEnd() && !v.isStart())
                graph->markAs(v, gr::pointBifurcation);
            graph->markAs(node.get(), gr::pointShortest);
            traverse(node.get());
        });
}

bool Dijkstra::completed() const
{
    return unvisited.empty()
        || unvisited.begin()->distIsInfinite()
        || dst.has_value()
        || !graph;
}

gr::Graph::VertexType Dijkstra::extractFirst()
{
    return unvisited.extract(*unvisited.begin()).value();
}
