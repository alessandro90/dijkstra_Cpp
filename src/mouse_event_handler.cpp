#include "mouse_event_handler.hpp"
#include "SFML/Graphics.hpp"
#include "graph.hpp"

namespace {

MousePos getMousePos(sf::Event const& event)
{
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return { .x = event.mouseButton.x, .y = event.mouseButton.y };
    case sf::Event::MouseMoved:
        return { .x = event.mouseMove.x, .y = event.mouseMove.y };
    default:
        return {};
    }
}

}
void MouseEventHandler::handleEvent(sf::Event const& event, gr::Vertex& ver)
{
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        MouseEventHandler::handleButtonEvent(event, ver);
        break;
    case sf::Event::MouseMoved:
        MouseEventHandler::handleMoveEvent(event, ver);
        break;
    default:
        break;
    }
}

void MouseEventHandler::handleButtonEvent(sf::Event const& event, gr::Vertex& ver)
{
    switch (ver.type()) {
    case gr::pointStart:
    case gr::pointEnd:
        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            state = ver.isStart() ? MouseEventHandler::State::GRABBED_START : MouseEventHandler::State::GRABBED_END;
            v = &ver;
        } else if (event.type == sf::Event::MouseButtonReleased)
            state = MouseEventHandler::State::FREE;
        break;
    case gr::pointObstacle:
        if (event.mouseButton.button == sf::Mouse::Right
            && event.type == sf::Event::MouseButtonPressed)
            ver.setType(gr::pointEmpty);
        state = MouseEventHandler::State::FREE;
        break;
    case gr::pointEmpty:
        if (event.mouseButton.button == sf::Mouse::Left
            && event.type == sf::Event::MouseButtonPressed)
            ver.setType(gr::pointObstacle);
        state = MouseEventHandler::State::FREE;
        break;
    default:
        break;
    }
}

void MouseEventHandler::handleMoveEvent(sf::Event const& event, gr::Vertex& ver)
{
    (void)event;
    switch (ver.type()) {
    case gr::pointStart:
        if (state != MouseEventHandler::State::GRABBED_END)
            v = &ver;
        break;
    case gr::pointEnd:
        if (state != MouseEventHandler::State::GRABBED_START)
            v = &ver;
        break;
    case gr::pointObstacle:
        if (state == MouseEventHandler::State::FREE && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            ver.setType(gr::pointEmpty);
            state = MouseEventHandler::State::FREE;
        }
        break;
    case gr::pointEmpty:
        if (state == MouseEventHandler::State::FREE && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            ver.setType(gr::pointObstacle);
        else if (state == MouseEventHandler::State::GRABBED_START) {
            v->setType(gr::pointEmpty);
            v->setDist(gr::infinite);
            v = &ver;
            v->setType(gr::pointStart);
            v->setDist(gr::Distance { 0 });
        } else if (state == MouseEventHandler::State::GRABBED_END) {
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

void updateMouseEventHandler(MouseEventHandler& mouseEventHandler, Settings const& settings, sf::Event const& event, gr::Graph& graph)
{
    if (MousePos mPos = getMousePos(event);
        mPos.x >= 0
        && mPos.x <= settings.windowSize.width - 1
        && mPos.y >= 0
        && mPos.y <= settings.windowSize.height - 1
        && !(event.type == sf::Event::MouseMoved
            && !sf::Mouse::isButtonPressed(sf::Mouse::Left)
            && !sf::Mouse::isButtonPressed(sf::Mouse::Right))) {
        auto const [xPos, yPos] = std::pair {
            static_cast<int>(mPos.y / settings.cellSize.height),
            static_cast<int>(mPos.x / settings.cellSize.width)
        };
        mouseEventHandler.handleEvent(event, *graph.vertexPtr(gr::Position { gr::X { xPos }, gr::Y { yPos } }));
    }
}