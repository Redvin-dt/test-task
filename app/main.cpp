#include "sorter.h"

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "incorect number of arguments, excepted two, buf find "
                  << argc - 1 << '\n'; // TODO: rewrite on usage ...
        return 0;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    sort(inputFilename, outputFilename);

    return 0;
}
