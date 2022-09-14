#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }
    FileFinder finder = FileFinder();
    //finder.findFileInDirectory("test.txt", "d:\\");
    //finder.findFileInDirectory("testFile.txt", "d:\\",true);
    //finder.printPaths();
    finder.findFileInSystem("test.txt",false);
    finder.printPaths();
    //findFileInDirectory("test.txt", "c:\\");
    //findFileInSystem("test.txt",false);
    //fileFinder("test.txt", dirPath);

    return 0;
}