#include "include/level.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Need a filename" << std::endl;
        return -1;
    }
    lv::Level level { argv[1] };
    std::cout << level << '\n';
    writeLevel("testout.txt", level);
}
