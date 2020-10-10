#include "app.hpp"

void App::EditAction::poll(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        app.window.close();
    else if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::Escape) {
        app.graph.reset();
    } else if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::Enter) {
        app.djk.loadGraph(app.graph);
        app.transition(app.propagateAction);
    }
}

void App::EditAction::perform(sf::Event& event)
{
    updateMouseEventHandler(app.mouseEventHandler, app.settings, event, app.graph);
}

void App::PropagateAction::poll(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        app.window.close();
    else if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::Escape) {
        app.graph.reset();
        app.transition(app.editAction);
    }
}

void App::PropagateAction::perform(sf::Event& event)
{
    (void)event;
    if (clock.getElapsedTime().asMilliseconds() >= app.settings.timeStep) {
        clock.restart();
        if (app.djk.done()) {
            app.transition(app.markAction);
        }
    }
}

void App::MarkAction::poll(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        app.window.close();
    else if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::Escape) {
        app.graph.reset();
        app.transition(app.editAction);
    }
}

void App::MarkAction::perform(sf::Event& event)
{
    (void)event;
    app.djk.markShortestPaths();
    app.transition(app.waitAction);
}

void App::WaitAction::poll(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        app.window.close();
    else if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::Escape) {
        app.graph.reset();
        app.transition(app.editAction);
    }
}

void App::WaitAction::perform(sf::Event& event)
{
    (void)event;
}

App::App(Settings&& settings_, sf::RenderWindow& window_)
    : settings { std::move(settings_) }
    , window { window_ }
{
    if (std::holds_alternative<Grid>(settings.grid)) {
        auto const& [rows, cols] = std::get<Grid>(settings.grid);
        graph.buildEmpty(rows, cols);
    } else {
        graph.fromFile(std::get<std::string>(settings.grid));
    }
}

void App::run()
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            currentAction->poll(event);
        }
        currentAction->perform(event);
        drawGrid(graph, window, settings.cellSize);
        window.display();
    }
}

void App::transition(Action& action)
{
    currentAction = &action;
}
