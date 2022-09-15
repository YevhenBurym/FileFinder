#pragma once

#include <string>
#include <filesystem>
#include <vector>

class FileFinder {
private:
    std::string fileName;
    std::vector<std::string> *filePaths;
    bool isFirstFound;
    bool isSearchInSystem;
    bool findFileInSubDirectory(const std::filesystem::directory_entry &directory);
public:
    FileFinder();
    ~FileFinder();
    bool findFileInCurrentDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                    bool onlyFirstFound = true);
    bool findFileInSystem(const std::string &inFileName, bool onlyFirstFound = true);
    void printPaths();
    std::vector<std::string> *getFilePath() const;
};