#include "SFML/Graphics.hpp"
#include "colors.hpp"
#include "config_parser.hpp"
#include "dijkstra.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
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

std::pair<int, int> getWindowSize(std::pair<unsigned, unsigned> squares, unsigned edgeSizeWidth, unsigned edgeSizeHeigh)
{
    return { static_cast<int>(squares.first * edgeSizeWidth), static_cast<int>(squares.second * edgeSizeHeigh) };
}

sf::RectangleShape& getRectangle(gr::Vertex const& v, unsigned edgeWidth, unsigned edgeHeight)
{
    constexpr static unsigned border { 2 };

    static sf::RectangleShape rectangle(sf::Vector2f(edgeWidth - border, edgeHeight - border));
    rectangle.setOutlineThickness(1.f);
    rectangle.setOutlineColor(sf::Color(150, 150, 150));
    rectangle.setPosition(sf::Vector2f {
        static_cast<float>(v.pos().y.value() * edgeWidth + border / 2),
        static_cast<float>(v.pos().x.value() * edgeHeight + border / 2) });
    return rectangle;
}

void drawCell(gr::Vertex const& v, sf::RenderWindow& window, unsigned edgeWidth, unsigned edgeHeight)
{
    static unsigned maxDistance { 0 };

    auto& rectangle = getRectangle(v, edgeWidth, edgeHeight);

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
        maxDistance = std::max<unsigned>(maxDistance, v.dist().value());
        rectangle.setFillColor(colorFromGradient(static_cast<unsigned>(v.dist().value()), maxDistance));
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

void drawGrid(auto const& nodes, sf::RenderWindow& window, unsigned edgeWidth, unsigned edgeHeight)
{
    for (auto const& nodeRow : nodes)
        for (auto const& node : nodeRow)
            drawCell(node, window, edgeWidth, edgeHeight);
}

auto getStartingConfiguration(int argc, char** argv)
{
    (void)argv;
    gr::Graph graph {};
    unsigned edgeWidth {};
    unsigned edgeHeight {};
    ConfigParser config {};
    int milli {};
    if (argc == 2 && std::string_view { argv[1] } == "-i") {
        config.parse("../text_files/config_i.txt");
        graph.buildEmpty(std::stoul(config.get("rows").data()),
            std::stoul(config.get("cols").data()));
    } else {
        config.parse("../text_files/config.txt");
        try {
            graph.loadFile(config.get("graphPath"));
        } catch (gr::InvalidGraphException const& e) {
            std::cerr << e.what() << '\n';
            throw;
        }
    }
    edgeWidth = std::stoul(config.get("edgeWidth").data());
    edgeHeight = std::stoul(config.get("edgeHeight").data());
    milli = std::atoi(config.get("maxFrameRate").data());
    return std::tuple(graph, edgeWidth, edgeHeight, milli);
}

class Mouse {
public:
    void handleEvent(sf::Event const& event, gr::Vertex& ver)
    {
        switch (event.type) {
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            handleButtonEvent(event, ver);
            break;
        case sf::Event::MouseMoved:
            handleMoveEvent(event, ver);
            break;
        default:
            break;
        }
    }

private:
    enum class State {
        GRABBED_START,
        GRABBED_END,
        FREE,
    };
    void handleButtonEvent(sf::Event const& event, gr::Vertex& ver)
    {
        switch (ver.type()) {
        case gr::pointStart:
        case gr::pointEnd:
            if (event.type == sf::Event::MouseButtonPressed)
                state = ver.isStart() ? State::GRABBED_START : State::GRABBED_END;
            else if (event.type == sf::Event::MouseButtonReleased)
                state = State::FREE;
            break;
        case gr::pointObstacle:
            if (event.mouseButton.button == sf::Mouse::Right
                && event.type == sf::Event::MouseButtonPressed)
                ver.setType(gr::pointEmpty);
            break;
        case gr::pointEmpty:
            if (event.mouseButton.button == sf::Mouse::Left
                && event.type == sf::Event::MouseButtonPressed)
                ver.setType(gr::pointObstacle);
            break;
        default:
            break;
        }
    }

    void handleMoveEvent(sf::Event const& event, gr::Vertex& ver)
    {
        (void)event;
        switch (ver.type()) {
        case gr::pointStart:
            if (state != State::GRABBED_END)
                v = &ver;
            break;
        case gr::pointEnd:
            if (state != State::GRABBED_START)
                v = &ver;
            break;
        case gr::pointObstacle:
            if (state == State::FREE && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                ver.setType(gr::pointEmpty);
                state = State::FREE;
            }
            break;
        case gr::pointEmpty:
            if (state == State::FREE && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                ver.setType(gr::pointObstacle);
            else if (state == State::GRABBED_START) {
                v->setType(gr::pointEmpty);
                v->setDist(gr::infinite);
                v = &ver;
                v->setType(gr::pointStart);
                v->setDist(gr::Distance { 0 });
            } else if (state == State::GRABBED_END) {
                v->setType(gr::pointEmpty);
                v = &ver;
                v->setType(gr::pointEnd);
                v->setDist(gr::infinite);
            }
            break;
        default:
            break;
        }
    }

    State state { State::FREE };
    gr::Vertex* v { nullptr };
};

void updateGraphInteractive(gr::Graph& graph, sf::Event const& event, Mouse& mouse, unsigned edgeWidth, unsigned edgeHeight)
{
    if (event.type == sf::Event::MouseMoved
        && !sf::Mouse::isButtonPressed(sf::Mouse::Left)
        && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
        return;

    int x {}, y {};
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        x = event.mouseButton.x;
        y = event.mouseButton.y;
    } else if (event.type == sf::Event::MouseMoved) {
        x = event.mouseMove.x;
        y = event.mouseMove.y;
    }

    auto const [xPos, yPos] = std::pair { static_cast<int>(y / edgeHeight), static_cast<int>(x / edgeWidth) };
    auto* vPtr = graph.vertexPtr(gr::Position { gr::X { xPos }, gr::Y { yPos } });
    mouse.handleEvent(event, *vPtr);
}

int main(int argc, char** argv)
{

    auto [graph, edgeWidth, edgeHeight, milli] = getStartingConfiguration(argc, argv);

    auto const [sizeX, sizeY] = getWindowSize(getSquresNumber(graph), edgeWidth, edgeHeight);

    sf::RenderWindow window {
        sf::VideoMode { static_cast<unsigned>(sizeX), static_cast<unsigned>(sizeY) },
        "Dijkstra",
        sf::Style::Close
    };

    bool started { false };
    bool finished { false };
    bool marked { false };
    Mouse mouse {};
    Dijkstra djk {};
    sf::Clock clock {};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape) {
                started = false;
                marked = false;
                finished = false;
                auto t = getStartingConfiguration(argc, argv);
                graph = std::get<0>(t);
            }

            if (!started
                && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Enter) {
                djk.loadGraph(graph);
                started = true;
            }

            if (!started
                && (event.type == sf::Event::MouseButtonPressed
                    || event.type == sf::Event::MouseButtonReleased
                    || event.type == sf::Event::MouseMoved)) {
                auto mousePos = sf::Mouse::getPosition(window);
                if (mousePos.x >= 0 && mousePos.x <= sizeX && mousePos.y >= 0 && mousePos.y <= sizeY)
                    updateGraphInteractive(graph, event, mouse, edgeWidth, edgeHeight);
            }
        }

        // Dijkstra propagation
        if (started && !marked) {
            if (clock.getElapsedTime().asMilliseconds() >= milli) {
                clock.restart();
                if (!finished) {
                    finished = djk.done();
                }
                if (finished) {
                    marked = true;
                    djk.markShortestPaths();
                }
            }
        }

        drawGrid(graph.nodes(), window, edgeWidth, edgeHeight);

        window.display();
    }
}
