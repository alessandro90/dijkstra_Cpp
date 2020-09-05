#include "dijkstra.hpp"
#include "easy_print.hpp"
#include "graph.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Need a filename" << std::endl;
        return -1;
    }
    Graph graph { argv[1] };
    Dijkstra djk { graph };
    while (!djk.done())
        ;
    djk.marKShortestPaths();
    print(graph, '\n');
}
