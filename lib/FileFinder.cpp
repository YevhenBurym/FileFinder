#include "FileFinder.h"
#include <iostream>

JoinThreads::JoinThreads(std::vector<std::thread> &threads) : threads(threads) {}

JoinThreads::~JoinThreads() {
    for (auto &thread : threads) {
        if (thread.joinable())
            thread.join();
    }
}

void
FileFinder::findInDirectoriesRange(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end,
                                   std::atomic<bool> &done, std::mutex &mtx) {
    for (; (begin != end) && !done.load(); ++begin) {
        try {
            for (const auto &entry : std::filesystem::recursive_directory_iterator{*begin,std::filesystem::directory_options::skip_permission_denied}) {
                if (done.load() && this->isFirstFound) return;

                if (entry.path().filename() == this->fileName) {
                    std::filesystem::path p = entry.path();

                    mtx.lock();
                    this->filePaths->push_back(p.make_preferred().string());
                    if (!this->isFirstFound) {
                        std::cout << p.make_preferred().string() << std::endl;
                    }
                    mtx.unlock();

                    if (this->isFirstFound) {
                        done.store(true);
                        return;
                    }
                }
            }
        } catch (...) {}
    }
}

bool FileFinder::findInSubDirectories(std::vector<std::string> &subDirectories) {
    std::mutex mtx;
    bool status = false;
    unsigned int maxThreads = std::thread::hardware_concurrency();
    unsigned int numThreads = maxThreads;
    unsigned int dirsForThread = 1;

    if (subDirectories.size() < maxThreads) {
        numThreads = subDirectories.size();
    } else {
        dirsForThread = subDirectories.size() / numThreads;
    }

    std::atomic<bool> done(false);
    std::vector<std::thread> threads(numThreads - 1);

    {
        JoinThreads joiner(threads);
        auto startDir = subDirectories.begin();
        for (unsigned int i = 0; i < (numThreads - 1); ++i) {
            auto lastDir = startDir + dirsForThread;
            threads[i] = std::thread(&FileFinder::findInDirectoriesRange, this, startDir, lastDir, std::ref(done),
                                     std::ref(mtx));
            startDir = lastDir;
        }
        this->findInDirectoriesRange(startDir, subDirectories.end(), done, mtx);
    }

    if (done.load()) {
        status = true;
    }

    return status;
}

bool FileFinder::findInDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                 bool onlyFirstFound) {
    this->fileName = inFileName;
    this->isFirstFound = onlyFirstFound;
    std::vector<std::string> subDirectories;

    if (!this->filePaths->empty() && !this->isSearchInSystem) {
        this->filePaths->clear();
        this->filePaths->shrink_to_fit();
    }
    if (!is_directory(directoryPath)) {
        std::cout << directoryPath << " is a wrong directory. Searching can't be started." << std::endl;
        return false;
    }

    for (const auto &entry : std::filesystem::directory_iterator{directoryPath}) {
        if (!is_directory(entry)) {
            if (entry.path().filename() == inFileName) {
                std::filesystem::path p = entry.path();
                this->filePaths->push_back(p.make_preferred().string());
                if (onlyFirstFound) {
                    return true;
                }
            }
        } else {
            subDirectories.push_back(entry.path().string());
        }
    }
    std::cout <<"Searching in "<< directoryPath.string() <<"..." << std::endl;
    bool status = this->findInSubDirectories(subDirectories);

    return status;
}

bool FileFinder::findInSystem(const std::string &inFileName, bool onlyFirstFound) {
    this->isSearchInSystem = true;
    bool status = false;

    for (char drive = 'a'; drive <= 'z'; ++drive) {
        std::filesystem::path currentDirectory = std::string(1, drive) + ":\\";
        if (is_directory(currentDirectory)) {
            if (this->findInDirectory(inFileName, currentDirectory, onlyFirstFound)) {
                status = true;
                if (onlyFirstFound) {
                    return status;
                }
            }
        }
    }
    this->isSearchInSystem = false;
    return status;
}

FileFinder::FileFinder() {
    this->isSearchInSystem = false;
    this->isFirstFound = true;
    this->fileName = std::string();
    this->filePaths = new std::vector<std::string>();
}

FileFinder::~FileFinder() {
    delete this->filePaths;
}

void FileFinder::printPaths() {
    if (!this->filePaths->empty()) {
        if (this->isFirstFound) {
            std::cout << "File path is: " << "\t" << this->filePaths->front() << std::endl;
        } else {
            std::cout << std::endl << "File have next paths: " << std::endl;
            for (const auto &it: *this->filePaths) {
                std::cout << "\t" << it << std::endl;
            }
        }
    } else {
        std::cout << "File " << this->fileName << " is not found." << std::endl;
    }
}

std::vector<std::string> *FileFinder::getPaths() const {
    return this->filePaths;
}


