#include "FileFinder.h"
#include <iostream>

std::string FileFinder::findFileInSubDirectoryPar(std::vector<std::string> &dirVector, std::string &match) {
    struct find_element {
        void operator()(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string &match,
                   std::promise<std::string> *result, std::atomic<bool> *done_flag) {
            for (; (begin != end) && !done_flag->load(); ++begin) {
                try {
                    for (const auto &entry : std::filesystem::recursive_directory_iterator{*begin,std::filesystem::directory_options::skip_permission_denied}) {
                        if (done_flag->load()) return;

                        if (entry.path().filename() == match) {
                            std::filesystem::path p = entry.path();
                            result->set_value(p.make_preferred().string());
                            done_flag->store(true);
                            return;
                        }
                    }
                } catch (const std::exception &) {
                    std::cout << "Directory access error!!!" << "\n";
                }
            }
        }
    };
    unsigned int block_size = 1;
    unsigned int max_threads = std::thread::hardware_concurrency();
    unsigned int num_threads = max_threads;
    if (dirVector.size() < max_threads) {
        num_threads = dirVector.size();
    } else {
        block_size = dirVector.size() / num_threads;
    }
    std::promise<std::string> result;
    std::atomic<bool> done_flag(false);
    std::vector<std::thread> threads(num_threads - 1);
    {
        JoinThreads joiner(threads);
        auto block_start = dirVector.begin();
        for (unsigned int i = 0; i < (num_threads - 1); ++i) {
            auto block_end = block_start;
            std::advance(block_end, block_size);
            threads[i] = std::thread(find_element(), block_start, block_end, std::ref(match), &result, &done_flag);
            block_start = block_end;
        }
        find_element()(block_start, dirVector.end(), std::ref(match), &result, &done_flag);
    }
    if (!done_flag.load()) {
        return "File is not found.";
    }
    return result.get_future().get();
}


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

    if (!this->filePaths->empty() && !this->isSearchInSystem) {
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

bool FileFinder::findFileInCurrentDirectoryPar(const std::string &inFileName, const std::filesystem::path &directoryPath,
                                          bool onlyFirstFound) {
    this->fileName = inFileName;
    this->isFirstFound = onlyFirstFound;
    bool status = false;

    if (!this->filePaths->empty() && !this->isSearchInSystem) {
        this->filePaths->clear();
        this->filePaths->shrink_to_fit();
    }
    if (!is_directory(directoryPath)) {
        std::cout << directoryPath << " Wrong directory. Searching can't be started." << std::endl;
        return status;
    }

    std::vector<std::string> dyrectories;

    for (const auto &entry : std::filesystem::directory_iterator{directoryPath}) {
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
                dyrectories.push_back(entry.path().string());
            } catch (const std::exception &) {
                std::cout << "Directory access error!!!" << "\n";
            }
        }
    }
    std::cout << findFileInSubDirectoryPar(dyrectories, this->fileName) << std::endl;

    return status;
}

bool FileFinder::findFileInSystem(const std::string &inFileName, bool onlyFirstFound) {
    this->isSearchInSystem = true;
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
        std::cout << "|-----------printPaths--------------|" << std::endl;
        std::cout << "File " << this->fileName << " is not found." << std::endl;
        std::cout << "|-----------------------------------|" << std::endl;
    }
}

std::vector<std::string> *FileFinder::getFilePath() const {
    return this->filePaths;
}


