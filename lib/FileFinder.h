#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include <thread>
#include <future>
#include <atomic>

class JoinThreads {
private:
    std::vector<std::thread> &threads;
public:
    explicit JoinThreads(std::vector<std::thread> &threads) : threads(threads) {}

    ~JoinThreads() {
        for (auto &thread : threads) {
            if (thread.joinable())
                thread.join();
        }
    }
};

class FileFinder {
private:
    std::string fileName;
    std::vector<std::string> *filePaths;
    bool isFirstFound;
    bool isSearchInSystem;

    bool findFileInSubDirectory(const std::filesystem::directory_entry &directory);
    std::string findFileInSubDirectoryPar(std::vector<std::string>& dirVector, std::string& match);
public:
    FileFinder();

    ~FileFinder();

    bool findFileInCurrentDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                    bool onlyFirstFound = true);
    bool findFileInCurrentDirectoryPar(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                    bool onlyFirstFound = true);
    bool findFileInSystem(const std::string &inFileName, bool onlyFirstFound = true);

    void printPaths();

    std::vector<std::string> *getFilePath() const;
};