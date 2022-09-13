#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }

    fileFinder("test.txt", dirPath);

    return 0;
}