#include <iostream>
#include "lib/FileFinder.h"

int main(int argc, const char **argv) {
    FileFinder finder;
    std::filesystem::path dirPath;
    std::string fileName;
    std::string allParam;
    bool inDirectory = true;
    bool all = false;


    if (argc >= 2) {
        fileName = argv[1];
        if (argc >= 3) {
            dirPath = argv[2];
        }
        if (argc == 4) {
            allParam = argv[3];
        }
        if (allParam == "-all" || dirPath == "-all") {
            all = true;
        }

        if (dirPath == "" || dirPath == "-all") {
            inDirectory = false;
        }

        if (inDirectory) {
            if (all) {
                finder.findInDirectory(fileName, dirPath,false);
            } else {
                finder.findInDirectory(fileName, dirPath);
            }
        } else {
            if (all) {
                finder.findInSystem(fileName,false);
            }  else {
                finder.findInSystem(fileName);
            }
        }

        finder.printPaths();
    } else {
        std::cout << "Enter the file name" << std::endl;
        std::cout << "Example if you want to search the entire system for the first match: FileFinder.exe <file name>" << std::endl;
        std::cout << "Example if you want to search in the directory for the first match: FileFinder.exe <file name> <directory path>" << std::endl;
        std::cout << "Add the -all parameter in the end to search for all matches" << std::endl;
    }
    return 0;
}