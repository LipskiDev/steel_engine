#include "utils.h"
#include <cstdio>
#include <sstream>
#include <fstream>
#include <cstdlib>

std::string Utils::readFileToString(const char *filename)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
                  << filename << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string contents((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    return contents;
}