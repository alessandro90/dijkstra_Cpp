#include "settings.hpp"
#include "config_parser.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <iostream>
#include <string>

namespace {
CellsNumber getCells(std::string_view str)
{
    std::size_t maxCount {};
    std::size_t counter {};
    std::size_t lines {};
    for (char ch : str) {
        if (ch != '\n') {
            counter += 1;
        } else {
            lines += 1;
            if (maxCount < counter)
                maxCount = counter;
            counter = 0;
        }
    }
    return { .x = maxCount, .y = lines };
}
}

Settings getSettings(int argc, char** argv)
{
    ConfigParser config {};
    std::variant<Grid, std::string> grid;
    CellsNumber cellsNumber;
    if (argc == 2 && std::string_view { argv[1] } == "-i") {
        config.parse("../text_files/config_i.txt");
        grid = Grid {
            .rows = std::stoul(config.get("rows").data()),
            .cols = std::stoul(config.get("cols").data())
        };
        cellsNumber.x = static_cast<decltype(WindowSize::width)>(std::get<0>(grid).cols);
        cellsNumber.y = static_cast<decltype(WindowSize::height)>(std::get<0>(grid).rows);
    } else {
        config.parse("../text_files/config.txt");
        try {
            grid = std::string { config.get("graphPath") };
            cellsNumber = getCells(io::File { config.get("graphPath") }.read());
        } catch (gr::InvalidGraphException const& e) {
            std::cerr << e.what() << '\n';
            throw;
        }
    }
    CellSize const cellSize {
        .width = std::stoul(config.get("edgeWidth").data()),
        .height = std::stoul(config.get("edgeHeight").data())
    };

    return {
        .cellSize = cellSize,
        .windowSize = {
            static_cast<decltype(WindowSize::width)>(cellsNumber.x * cellSize.width),
            static_cast<decltype(WindowSize::height)>(cellsNumber.y * cellSize.height) },
        .grid = std::move(grid),
        .timeStep = std::atoi(config.get("maxFrameRate").data())
    };
}