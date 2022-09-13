#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }
    //findFileInDirectory("test.txt", dirPath);
    findFileInSystem("test.txt",false);
    //fileFinder("test.txt", dirPath);

    return 0;
}