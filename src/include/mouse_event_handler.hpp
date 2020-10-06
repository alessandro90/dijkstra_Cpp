#ifndef MOUSE_EVENT_HANDLER_HPP
#define MOUSE_EVENT_HANDLER_HPP

#include "SFML/Graphics.hpp"
#include "graph.hpp"
#include "settings.hpp"

struct MousePos {
    int x {};
    int y {};
};

class MouseEventHandler {
public:
    void handleEvent(sf::Event const& event, gr::Vertex& ver);

private:
    enum class State {
        GRABBED_START,
        GRABBED_END,
        FREE,
    };
    void handleButtonEvent(sf::Event const& event, gr::Vertex& ver);

    void handleMoveEvent(sf::Event const& event, gr::Vertex& ver);

    State state { State::FREE };
    gr::Vertex* v { nullptr };
};

void updateMouseEventHandler(MouseEventHandler& mouseEventHandler, Settings const& settings, sf::Event const& event, gr::Graph& graph);

#endif