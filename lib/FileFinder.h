#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include <thread>
#include <atomic>
#include <mutex>

class JoinThreads {
private:
    std::vector<std::thread> &threads;
public:
    explicit JoinThreads(std::vector<std::thread> &threads);
    ~JoinThreads();
};

class FileFinder {
private:
    std::string fileName;
    std::vector<std::string> *filePaths;
    bool isFirstFound;
    bool isSearchInSystem;
    bool findInSubDirectories(std::vector<std::string> &subDirectories);
    void findInDirectoriesRange(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end,
                                std::atomic<bool> &done, std::mutex &mtx);
public:
    FileFinder();
    ~FileFinder();
    bool findInDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath,
                         bool onlyFirstFound = true);
    bool findInSystem(const std::string &inFileName, bool onlyFirstFound = true);
    void printPaths();
    std::vector<std::string> *getPaths() const;
};