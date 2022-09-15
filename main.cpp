#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }
    FileFinder finder = FileFinder();
    //finder.findFileInSystem("testFile.txt",false);
    finder.findFileInCurrentDirectory("testFile.txt", "d:\\");
    finder.printPaths();

    return 0;
}