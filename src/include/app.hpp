#ifndef APP_HPP
#define APP_HPP
#include "SFML/Graphics.hpp"
#include "dijkstra.hpp"
#include "draw.hpp"
#include "graph.hpp"
#include "mouse_event_handler.hpp"
#include "settings.hpp"
#include <iostream>
#include <utility>

class App {

    struct Action {
        virtual void perform(sf::Event& event) = 0;
        virtual void poll(sf::Event& event) = 0;
        virtual ~Action() = default;
    };

    struct EditAction : public Action {
        EditAction(App& app_)
            : app { app_ }
        {
        }
        void poll(sf::Event& event) override;

        void perform(sf::Event& event) override;

    private:
        App& app;
    };

    struct PropagateAction : public Action {
        PropagateAction(App& app_)
            : app { app_ }
        {
        }
        void poll(sf::Event& event) override;

        void perform(sf::Event& event) override;

    private:
        App& app;
        sf::Clock clock {};
    };

    struct MarkAction : public Action {
        MarkAction(App& app_)
            : app { app_ }
        {
        }
        void poll(sf::Event& event) override;

        void perform(sf::Event& event) override;

    private:
        App& app;
    };

    struct WaitAction : public Action {
        WaitAction(App& app_)
            : app { app_ }
        {
        }
        void poll(sf::Event& event) override;

        void perform(sf::Event& event) override;

    private:
        App& app;
    };

public:
    App(Settings&& settings_, sf::RenderWindow& window_);

    void run();

private:
    void transition(Action& action);
    Settings settings;
    sf::RenderWindow& window;
    gr::Graph graph {};
    MouseEventHandler mouseEventHandler {};
    // Dijkstra algorithm
    Dijkstra djk {};
    // Possible states
    EditAction editAction { *this };
    PropagateAction propagateAction { *this };
    MarkAction markAction { *this };
    WaitAction waitAction { *this };
    Action* currentAction { &editAction };
};

#endif