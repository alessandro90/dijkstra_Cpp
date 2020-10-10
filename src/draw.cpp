#ifndef DRAW_HPP
#define DRAW_HPP

#include "draw.hpp"
#include "SFML/Graphics.hpp"
#include "colors.hpp"
#include "graph.hpp"
#include "settings.hpp"
#include <vector>

namespace {

sf::RectangleShape& getRectangle(gr::Vertex const& v, CellSize const& cellSize)
{
    constexpr static unsigned border { 2 };

    static sf::RectangleShape rectangle(sf::Vector2f(cellSize.width - border, cellSize.height - border));
    rectangle.setOutlineThickness(1.f);
    rectangle.setOutlineColor(sf::Color(150, 150, 150));
    rectangle.setPosition(sf::Vector2f {
        static_cast<float>(v.pos().y.value() * cellSize.width + border / 2),
        static_cast<float>(v.pos().x.value() * cellSize.height + border / 2) });
    return rectangle;
}

void drawCell(gr::Vertex const& v, sf::RenderWindow& window, CellSize const& cellSize, gr::Distance maxDistance)
{
    auto& rectangle = getRectangle(v, cellSize);

    switch (v.type()) {
    case gr::pointEmpty:
        rectangle.setFillColor(emptyColor);
        break;
    case gr::pointObstacle:
        rectangle.setFillColor(obstacleColor);
        break;
    case gr::pointBifurcation:
    case gr::pointShortest:
        rectangle.setFillColor(shortestColor);
        break;
    case gr::pointVisited:
        rectangle.setFillColor(colorFromGradient(v.dist().value(), maxDistance.value()));
        break;
    case gr::pointFront:
        rectangle.setFillColor(frontColor);
        break;
    case gr::pointStart:
        rectangle.setFillColor(startColor);
        break;
    case gr::pointEnd:
        rectangle.setFillColor(endColor);
        break;
    }
    window.draw(rectangle);
}

}

void drawGrid(gr::Graph const& graph, sf::RenderWindow& window, CellSize const& cellSize)
{
    for (auto const& nodeRow : graph.nodes())
        for (auto const& node : nodeRow)
            drawCell(node, window, cellSize, graph.getMaxDistance());
}

#endif