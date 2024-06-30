#include "utils.h"
#include <cstdio>
#include <sstream>
#include <fstream>
#include <cstdlib>

std::string Utils::ReadFileToString(const char *file_name)
{
    std::ifstream input_file(file_name);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
                  << file_name << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string contents((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    return contents;
}