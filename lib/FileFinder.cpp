#include "FileFinder.h"
#include <iostream>

//----------------------------------------------------------------------//
//Функция для поиска файла в поддиректории(рекурсивно). на вход имя файла и путь где искать.
//в цикле перебираются перебираются рекурсивно все директории и файлы directory и
//проверяется на соответствие заданному имени файла. если файл найден, то возращает true, иначе false.
bool findFileInSubDir(const std::string &fileName, const std::filesystem::directory_entry &directory) {
    for (const auto &entry : std::filesystem::recursive_directory_iterator{directory,std::filesystem::directory_options::skip_permission_denied}) {
        if (entry.path().filename() == fileName) {
            std::cout << "File path is: " << std::endl;
            std::filesystem::path p = entry.path();
            std::cout << p.make_preferred().string() << std::endl;

            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------//
//Функция для поиска файла в выбранной директории. на вход имя и путь где искать
//в цикле перебираются все ДИРЕКТОРИИ в выбранной dirPath.
// если это не директория, то проверяется на соответствие заданному имени файла, может это и есть искомый файл.
//и возвращает true, если файл найден.
// Иначе пробуем вызывать функцию findFileInSubDir для поиска внутри директории.
// Если результат работы findFileInSubDir является true, то вызывается true здесь.
// иначе сообщение, что файл не найден и следующий шаг цикла.
bool findFileInDirectory(const std::string &fileName, const std::filesystem::path &dirPath) {
    if (!is_directory(dirPath)) {
        std::cout << dirPath << " Wrong directory. Searching can't be started." << std::endl;
        return false;
    }
    for (const auto &entry : std::filesystem::directory_iterator{dirPath}) {
        std::cout << "---Checking--- " << entry.path().string() << "\n";
        if (!is_directory(entry)) {
            if (entry.path().filename() == fileName) {
                std::cout << "  File path is: " << std::endl;
                std::filesystem::path p = entry.path();
                std::cout << p.make_preferred().string() << std::endl;
                return true;
            }
        } else {
            try {
                if (findFileInSubDir(fileName, entry)) {
                    return true;
                }
            } catch (const std::exception &) {
                std::cout << "Directory access error!!!" << "\n";
            }
            std::cout << "  File is not found." << "\n";
        }
    }
    std::cout << "File is not found in this directory." << "\n";
    return false;
}
//----------------------------------------------------------------------//
//Функция для поиска файла в системе. на вход имя файла.
//Проверяет правильность пути и перебирает диски. поиском занимается findFileInDir, которая вызывается изнутри.
//Перебор дисков происходит по алфавиту.
//Создание новой строки формата пути "[drive]:\" при каждой итерации цикла;
//И проверка если это директория(не вышли за диск "z:\"),
//То вызвать функцию findFileInDir для поиска в директории. Return, так как ищем первое упоминание.
// если найден файл, то выйти из функции. Если не найден, то выведет сообщение.
void findFileInSystem(const std::string &fileName, bool isFirstFound) {
    for (char drive = 'a'; drive <= 'z'; ++drive) {
        std::filesystem::path currentDirectory = std::string(1, drive) + ":\\";
        if (is_directory(currentDirectory)) {
            if (findFileInDirectory(fileName, currentDirectory)) {
                if (isFirstFound) {
                    return;
                }
            }
        }
    }
    std::cout << "File is not found." << "\n";
}

bool FileFinder::findFileInSubDirectory(const std::filesystem::directory_entry& directory) {
    bool status = false;

    for (const auto &entry : std::filesystem::recursive_directory_iterator{directory,std::filesystem::directory_options::skip_permission_denied}) {
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

bool FileFinder::findFileInCurrentDirectory(const std::string &inFileName, const std::filesystem::path &directoryPath, bool onlyFirstFound) {
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

bool FileFinder::findFileInSystem(const std::string &inFileName, bool onlyFirstFound) {
    this->isSearchInSystem = true;
    bool status = false;

    if (!this->filePaths->empty()) {
        this->filePaths->clear();
        this->filePaths->shrink_to_fit();
    }

    for (char drive = 'a'; drive <= 'z'; ++drive) {
        std::filesystem::path currentDirectory = std::string(1, drive) + ":\\";
        if (is_directory(currentDirectory)) {
            if (this->findFileInCurrentDirectory(inFileName, currentDirectory, onlyFirstFound)) {
                status = true;
                if (onlyFirstFound) {
                    this->isSearchInSystem = false;
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
                std::cout << "File "<< this->fileName <<" path is: " << std::endl;
                std::cout << this->filePaths->front() << std::endl;
        } else {
            std::cout << "File "<< this->fileName <<" have next paths: " << std::endl;
            for (const auto& it: *this->filePaths) {
                std::cout << it << std::endl;
            }
        }
    } else {
        std::cout <<"File "<< this->fileName <<" is not found." << std::endl;
    }
}

std::vector<std::string>* FileFinder::getFilePath() const {
    return this->filePaths;
}


