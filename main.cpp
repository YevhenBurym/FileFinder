#include <iostream>
#include "lib/FileFinder.h"
#include "SimpleTimer/SimpleTimer.h"

int main(int argc, const char **argv) {
    std::filesystem::path dirPath;
    if (argc >= 2) {
        dirPath = argv[1];
    }
    FileFinder finder;
    //finder.findFileInSystem("testFile.txt",false);
    SimpleTimer tim;
    finder.findFileInCurrentDirectoryPar("test.txt", "c:\\");
    finder.printPaths();
    return 0;
}