#include "limit_allocator.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using TCommonAllocator = limit_allocator<int>; // T = any type except void
using TStringAllocator = limit_allocator<char, TCommonAllocator::rebind_alloc<char>>;
using TString = std::basic_string<char, std::char_traits<char>, TStringAllocator>;
using TVectorAllocator = limit_allocator<TString, TCommonAllocator::rebind_alloc<TString>>;

int main() {
    TCommonAllocator common_allocator(0, 0);
    TStringAllocator string_allocator(0, 0, common_allocator);
    TVectorAllocator vector_allocator(0, 0, common_allocator);

    std::vector<TString, TVectorAllocator> v(vector_allocator);
    std::cerr << "common: " << common_allocator.get_usage() << " string: " << string_allocator.get_usage() << " vector: " << vector_allocator.get_usage() << std::endl;
    v.emplace_back(TString("1", string_allocator));
    std::cerr << "common: " << common_allocator.get_usage() << " string: " << string_allocator.get_usage() << " vector: " << vector_allocator.get_usage() << std::endl;
    v.emplace_back(TString("12", string_allocator));
    std::cerr << "common: " << common_allocator.get_usage() << " string: " << string_allocator.get_usage() << " vector: " << vector_allocator.get_usage() << std::endl;
    v.emplace_back(TString("123", string_allocator));
    std::cerr << "common: " << common_allocator.get_usage() << " string: " << string_allocator.get_usage() << " vector: " << vector_allocator.get_usage() << std::endl;
    TString x("qwesdjnflkjsdnjanbsdfjhbasjdhfbakjhshdbfhjabsdjfhabsdjhfbjahsdbfjabsdjhfbashjhdbfjasbdfjbasdjfbajshdbf", string_allocator);
    std::cerr << "common: " << common_allocator.get_usage() << " string: " << string_allocator.get_usage() << " vector: " << vector_allocator.get_usage() << std::endl;
}
