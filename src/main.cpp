#include "dijkstra.hpp"
#include "easy_print.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <iostream>
#include <string>

int readSquareSize()
{
    return std::stoi(io::File { "../src/config.txt" }.read());
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Need a filename" << std::endl;
        return -1;
    }
    gr::Graph graph {};
    try {
        graph.loadFile(argv[1]);
    } catch (gr::InvalidGraphException const& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    Dijkstra djk { graph };
    while (!djk.done())
        ;
    djk.marKShortestPaths();
}
