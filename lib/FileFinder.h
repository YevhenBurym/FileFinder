#pragma once

#include <string>
#include <filesystem>
#include <vector>

bool findFileInSubDir(const std::string& fileName, const std::filesystem::directory_entry& directory);
bool findFileInDirectory(const std::string& fileName, const std::filesystem::path& dirPath);
void findFileInSystem(const std::string& fileName, bool isFirstFound);


class FileFinder {
private:
    std::string fileName;
    //std::string filePath;
    std::vector<std::string>* filePaths;
    bool isFirstFound;
    bool findFileInSubDirectory(const std::filesystem::directory_entry& directory);
public:
    FileFinder();
    ~FileFinder();
    bool findFileInCurrentDirectory(const std::string& inFileName, const std::filesystem::path& directoryPath, bool onlyFirstFound = true);
    bool findFileInSystem(const std::string& inFileName, bool onlyFirstFound= true);
    void printPaths();
    std::vector<std::string>* getFilePath() const;
};