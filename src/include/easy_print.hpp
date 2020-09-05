#ifndef EASY_PRINT_HPP
#define EASY_PRINT_HPP

#include <iostream>
#include <utility>

template <typename... Args>
void print(Args&&... args)
{
    (std::cout << ... << std::forward<Args>(args));
}

#endif