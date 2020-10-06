#ifndef DRAW_HPP
#define DRAW_HPP

#include "SFML/Graphics.hpp"
#include "graph.hpp"
#include "settings.hpp"
#include <vector>

void drawGrid(std::vector<std::vector<gr::Graph::VertexType>> const& nodes, sf::RenderWindow& window, CellSize const& cellSize);

#endif