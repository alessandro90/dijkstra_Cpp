#ifndef COLORS_HPP
#define COLORS_HPP
#include "SFML/Graphics.hpp"

inline sf::Color const emptyColor { 250, 250, 250 };
inline sf::Color const obstacleColor { 0, 0, 0 };
inline sf::Color const shortestColor { 157, 60, 249 };
inline sf::Color const frontColor { 255, 182, 108 };
inline sf::Color const startColor { 0, 0, 255 };
inline sf::Color const endColor { 255, 0, 0 };

namespace {

sf::Color colorFromGradient(double dist, double maxDist)
{
    const unsigned char c = 255 - static_cast<unsigned char>(255. * dist / maxDist);
    return sf::Color { c, 255, c };
}

}
#endif