#include "FileFinder.h"
#include <iostream>

bool findFileInSubDir(const std::string& fileName, const std::filesystem::directory_entry& directory) {
    for(const auto& entry : std::filesystem::recursive_directory_iterator{ directory, std::filesystem::directory_options::skip_permission_denied }) {
        if (entry.path().filename() == fileName) {
            std::cout << "File path is: " << std::endl;
            std::filesystem::path p = entry.path();
            std::cout << p.make_preferred().string() << std::endl;

            return true;
        }
    }
    return false;
}

bool findFileInDir(const std::string& fileName, const std::filesystem::path& dirPath) {
    for (const auto &entry : std::filesystem::directory_iterator{dirPath}) {
        try {
            if (findFileInSubDir(fileName, entry)) {
                return true;
            }
            std::cout << " " << entry.path().string() << "\n";
        } catch (const std::exception &) {
        }
    }
    return false;
}

void fileFinder(const std::string& fileName, std::filesystem::path& startDirPath) {
    int offset = 'a' - 'A';

    if ( startDirPath.string()[0] >= 'A' && startDirPath.string()[0] <= 'Z' ) {
        std::string p = startDirPath.string();
        p.front() = startDirPath.string()[0] + offset;
        startDirPath = p;
    }

    if (!is_directory(startDirPath)) {
        std::cout << startDirPath << " is invalid path." << std::endl;
        startDirPath = "c:\\";
        std::cout << "Default directory is c:\\" << std::endl;
    }

    if (findFileInDir(fileName, startDirPath)) {
        return;
    }

    for( char drive = 'd'; drive <= 'z'; ++drive ) {
        std::filesystem::path nextDir = std::string(1, drive) + ":\\";

        if (drive == startDirPath.string()[0] ) {
            nextDir = std::string(1, ++drive) + ":\\";
        }

        if (is_directory(nextDir)) {
            if (findFileInDir(fileName, nextDir)) {
                return;
            }
        }
    }
    std::cout << "File not found."<< "\n";
}
