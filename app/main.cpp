#include "sorter.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "incorect number of arguments, excepted two, buf find "
                  << argc - 1 << '\n';
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    sort(input_filename, output_filename);

    return 0;
}
