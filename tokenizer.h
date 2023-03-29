#include <vector>
#include <string>
#include <iostream>

#include <dlfcn.h>
#include <cstdio>

const std::vector<int32_t> encode(std::string input);
std::string decode(const std::vector<int32_t> ids);