#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }

    fileFinder("amoeba.c", dirPath);

    return 0;
}