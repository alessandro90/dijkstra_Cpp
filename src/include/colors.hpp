#ifndef COLORS_HPP
#define COLORS_HPP
#include "SFML/Graphics.hpp"
#include <array>
#include <cmath>

// inline std::array const colors = {
//     sf::Color { 109, 254, 46 },
//     sf::Color { 73, 252, 61 },
//     sf::Color { 0, 250, 74 },
//     sf::Color { 0, 248, 87 },
//     sf::Color { 0, 245, 99 },
//     sf::Color { 0, 243, 111 },
//     sf::Color { 0, 240, 123 },
//     sf::Color { 0, 237, 135 },
//     sf::Color { 0, 234, 147 },
//     sf::Color { 0, 230, 158 },
//     sf::Color { 0, 227, 170 },
//     sf::Color { 0, 223, 182 },
//     sf::Color { 0, 220, 194 },
//     sf::Color { 0, 216, 205 },
//     sf::Color { 0, 212, 217 },
//     sf::Color { 0, 208, 228 },
//     sf::Color { 0, 204, 238 },
//     sf::Color { 0, 200, 249 },
//     sf::Color { 0, 196, 255 },
//     sf::Color { 0, 192, 255 },
//     sf::Color { 0, 187, 255 },
//     sf::Color { 0, 183, 255 },
//     sf::Color { 0, 179, 255 },
//     sf::Color { 0, 174, 255 },
//     sf::Color { 0, 169, 255 },
//     sf::Color { 0, 164, 255 },
//     sf::Color { 0, 159, 255 },
//     sf::Color { 0, 153, 255 },
//     sf::Color { 0, 148, 255 },
//     sf::Color { 0, 141, 255 },
//     sf::Color { 0, 135, 255 },
//     sf::Color { 0, 128, 255 },
//     sf::Color { 0, 121, 255 },
//     sf::Color { 0, 113, 255 },
//     sf::Color { 0, 105, 255 },
//     sf::Color { 0, 96, 255 },
//     sf::Color { 0, 86, 255 },
//     sf::Color { 0, 75, 255 },
//     sf::Color { 0, 63, 254 },
//     sf::Color { 48, 47, 242 }
// };

inline sf::Color const emptyColor { 250, 250, 250 };
inline sf::Color const obstacleColor { 0, 0, 0 };
inline sf::Color const shortestColor { 157, 60, 249 };
inline sf::Color const frontColor { 255, 182, 108 };
inline sf::Color const startColor { 0, 0, 255 };
inline sf::Color const endColor { 255, 0, 0 };

namespace {
// sf::Color const& colorFromGradient(unsigned dist, unsigned maxDist)
// {
//     return colors[((colors.size() - 1) * dist) / maxDist];
// }
sf::Color colorFromGradient(unsigned dist, unsigned maxDist)
{
    static constexpr unsigned short minHue { 0 };
    static constexpr unsigned short maxHue { 248 };
    static constexpr unsigned short saturation { 83 };
    static constexpr unsigned short lightness { 64 };
    unsigned short const hue = ((maxHue - minHue) * dist) / maxDist + minHue;

    unsigned short const chroma = (1 - std::abs(2 * lightness - 1)) * saturation;
    unsigned short const hueP = hue / 60;
    unsigned short const x = chroma * (1 - std::abs(hueP % 2 - 1));
    unsigned short rp, gp, bp;
    if (hueP <= 1) {
        rp = chroma;
        gp = x;
        bp = 0;
    } else if (hueP <= 2) {
        rp = x;
        gp = chroma;
        bp = 0;
    } else if (hueP <= 3) {
        rp = 0;
        gp = chroma;
        bp = x;
    } else if (hueP <= 4) {
        rp = 0;
        gp = x;
        bp = chroma;
    } else if (hueP <= 5) {
        rp = x;
        gp = 0;
        bp = chroma;
    } else if (hueP <= 6) {
        rp = chroma;
        gp = 0;
        bp = x;
    } else {
        rp = 0;
        gp = 0;
        bp = 0;
    }

    unsigned short const m = lightness - chroma / 2;
    unsigned char const R = rp + m;
    unsigned char const G = gp + m;
    unsigned char const B = bp + m;
    return sf::Color { R, G, B };
}
}
#endif