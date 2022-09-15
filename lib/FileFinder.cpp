#include "FileFinder.h"
#include <iostream>

bool FileFinder::findFileInSubDirectory(const std::filesystem::directory_entry &directory) {
    bool status = false;

    for (const auto &entry : std::filesystem::recursive_directory_iterator{directory,
                                                                           std::filesystem::directory_options::skip_permission_denied}) {
        if (entry.path().filename() == this->fileName) {
            std::filesystem::path p = entry.path();
            this->filePaths->push_back(p.make_preferred().string());
            status = true;
            if (this->isFirstFound) {
                return status;
            }
        }
    }
    return status;
}

bool FileFinder::findFileInCurrentDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                            bool onlyFirstFound) {
    this->fileName = inFileName;
    this->isFirstFound = onlyFirstFound;
    bool status = false;

    if (!this->filePaths->empty()) {
        this->filePaths->clear();
        this->filePaths->shrink_to_fit();
    }
    if (!is_directory(directoryPath)) {
        std::cout << directoryPath << " Wrong directory. Searching can't be started." << std::endl;
        return status;
    }

    for (const auto &entry : std::filesystem::directory_iterator{directoryPath}) {
        std::cout << "Checking---> " << entry.path().string() << "\n";
        if (!is_directory(entry)) {
            if (entry.path().filename() == inFileName) {
                std::filesystem::path p = entry.path();
                this->filePaths->push_back(p.make_preferred().string());
                status = true;
                if (onlyFirstFound) {
                    return status;
                }
            }
        } else {
            try {
                if (this->findFileInSubDirectory(entry)) {
                    status = true;
                    if (onlyFirstFound) {
                        return status;
                    }
                }
            } catch (const std::exception &) {
                std::cout << "Directory access error!!!" << "\n";
            }
        }
    }
    return status;
}

bool FileFinder::findFileInSystem(const std::string &inFileName, bool onlyFirstFound) {
    bool status = false;

    for (char drive = 'a'; drive <= 'z'; ++drive) {
        std::filesystem::path currentDirectory = std::string(1, drive) + ":\\";
        if (is_directory(currentDirectory)) {
            if (this->findFileInCurrentDirectory(inFileName, currentDirectory, onlyFirstFound)) {
                status = true;
                if (onlyFirstFound) {
                    return status;
                }
            }
        }
    }
    return status;
}

FileFinder::FileFinder() {
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
            std::cout << "|-----------------------------------|" << std::endl;
            std::cout << "File " << this->fileName << " path is: " << std::endl;
            std::cout << this->filePaths->front() << std::endl;
            std::cout << "|-----------------------------------|" << std::endl;
        } else {
            std::cout << "|-----------------------------------|" << std::endl;
            std::cout << "File " << this->fileName << " have next paths: " << std::endl;
            for (const auto &it: *this->filePaths) {
                std::cout << it << std::endl;
            }
            std::cout << "|-----------------------------------|" << std::endl;
        }
    } else {
        std::cout << "|-----------------------------------|" << std::endl;
        std::cout << "File " << this->fileName << " is not found." << std::endl;
        std::cout << "|-----------------------------------|" << std::endl;
    }
}

std::vector<std::string> *FileFinder::getFilePath() const {
    return this->filePaths;
}


