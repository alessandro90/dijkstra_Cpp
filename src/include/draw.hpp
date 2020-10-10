#ifndef DRAW_HPP
#define DRAW_HPP

#include "SFML/Graphics.hpp"
#include "graph.hpp"
#include "settings.hpp"
#include <vector>

void drawGrid(gr::Graph const& graph, sf::RenderWindow& window, CellSize const& cellSize);

#endif