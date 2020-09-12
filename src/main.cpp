#include "SFML/Graphics.hpp"
#include "colors.hpp"
#include "config_parser.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

std::pair<unsigned, unsigned> getSquresNumber(gr::Graph& graph)
{
    auto const& nodes = graph.nodes();
    unsigned height = nodes.size();
    unsigned width = std::max_element(nodes.begin(), nodes.end(), [](auto const& n1, auto const& n2) {
        return n1.size() < n2.size();
    })->size();
    return { width, height };
}

std::pair<unsigned, unsigned> getWindowSize(std::pair<unsigned, unsigned> squares, unsigned edgeSizeWidth, unsigned edgeSizeHeigh)
{
    return { squares.first * edgeSizeWidth, squares.second * edgeSizeHeigh };
}

void drawSquare(gr::Vertex const& v, sf::RenderWindow& window, unsigned edgeWidth, unsigned edgeHeight)
{
    static std::size_t maxDistance { 0 };

    sf::RectangleShape rectangle(sf::Vector2f(edgeWidth - 2, edgeHeight - 2));
    rectangle.setOutlineThickness(1.f);
    rectangle.setOutlineColor(sf::Color(150, 150, 150));
    rectangle.setPosition(sf::Vector2f {
        static_cast<float>(v.pos().y.value() * edgeWidth),
        static_cast<float>(v.pos().x.value() * edgeHeight) });
    sf::Color color { 100, 100, 100 };
    switch (v.type()) {
    case gr::pointEmpty:
        break;
    case gr::pointObstacle:
        color = sf::Color { 0, 0, 0 };
        break;
    case gr::pointBifurcation:
    case gr::pointShortest:
        color = sf::Color { 179, 108, 255 };
        break;
    case gr::pointVisited:
        maxDistance = std::max<std::size_t>(maxDistance, v.dist().value());
        color = colors[static_cast<std::size_t>(((colors.size() - 1) * v.dist().value()) / maxDistance)];
        break;
    case gr::pointFront:
        color = sf::Color { 255, 182, 108 };
        break;
    case gr::pointStart:
        color = sf::Color { 0, 0, 255 };
        break;
    case gr::pointEnd:
        color = sf::Color { 255, 0, 0 };
        break;
    }
    rectangle.setFillColor(color);
    window.draw(rectangle);
}

void drawGrid(auto const& nodes, sf::RenderWindow& window, unsigned edgeWidth, unsigned edgeHeight)
{
    for (auto const& nodeRow : nodes)
        for (auto const& node : nodeRow)
            drawSquare(node, window, edgeWidth, edgeHeight);
}

int main()
{

    ConfigParser config {};
    config.parse();
    gr::Graph graph {};
    try {
        graph.loadFile(config.get("graphPath"));
    } catch (gr::InvalidGraphException const& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    unsigned edgeWidth = std::stoul(config.get("edgeWidth").data());
    unsigned edgeHeight = std::stoul(config.get("edgeHeight").data());
    unsigned milli = std::stoul(config.get("maxFrameRate").data());

    auto windowSize = getWindowSize(getSquresNumber(graph), edgeWidth, edgeHeight);

    sf::RenderWindow window {
        sf::VideoMode { windowSize.first, windowSize.second },
        "Dijkstra",
        sf::Style::Close
    };

    auto const& nodes = graph.nodes();
    Dijkstra djk { graph };
    bool finished { false };
    bool marked { false };
    sf::Clock clock {};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asMilliseconds() >= static_cast<int>(milli)) {
            clock.restart();
            if (!finished)
                finished = djk.done();
            if (finished && !marked) {
                marked = true;
                djk.markShortestPaths();
            }
        }

        drawGrid(nodes, window, edgeWidth, edgeHeight);

        window.display();
    }
}
