#include "limit_allocator.hpp"

#include <vector>
#include <iostream>
#include <stdlib.h>

int main()
    {
    using Alloc = limit_allocator<void>;
    Alloc void_alloc(0, 1024);
    limit_allocator<char> char_alloc(0, 1024);
    using IntAlloc = limit_allocator<int>;
    IntAlloc int_alloc(char_alloc);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    char* char_p = char_alloc.allocate(1);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    int* int_p = int_alloc.allocate(1);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    char_alloc.deallocate(char_p, 1);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    std::vector<int,IntAlloc> v(int_alloc);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    v.push_back(0);
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    v.clear();
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    v.shrink_to_fit();
    std::cout << "used: " << char_alloc.get_usage() << std::endl;
    return EXIT_SUCCESS;
    }

