#ifndef DATA_UTIL
#define DATA_UTIL

#include <string>
#include <vector>

std::string ed_stringSection(std::string s, size_t start, size_t end);

std::vector<int> ed_parseNums(std::string input);

#endif