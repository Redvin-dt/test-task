#include "sorter.h"

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: <input file> <output file>\n";
        return 0;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    sort(inputFilename, outputFilename);

    return 0;
}
