#include "SFML/Graphics.hpp"
#include "app.hpp"
#include "settings.hpp"
#include <utility>

int main(int argc, char** argv)
{
    Settings settings = getSettings(argc, argv);

    sf::RenderWindow window {
        sf::VideoMode {
            static_cast<unsigned>(settings.windowSize.width),
            static_cast<unsigned>(settings.windowSize.height) },
        "Dijkstra",
        sf::Style::Close
    };

    App app { std::move(settings), window };

    app.run();
}
