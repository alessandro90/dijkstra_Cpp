#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <cstddef>
#include <string>
#include <variant>

struct CellSize {
    unsigned width {};
    unsigned height {};
};

struct CellsNumber {
    std::size_t x {};
    std::size_t y {};
};

struct Grid {
    unsigned int rows {};
    unsigned int cols {};
};

struct WindowSize {
    int width {};
    int height {};
};

struct Settings {
    CellSize cellSize {};
    WindowSize windowSize {};
    std::variant<Grid, std::string> grid {};
    int timeStep {};
};

Settings getSettings(int argc, char** argv);

#endif