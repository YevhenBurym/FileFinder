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
//----------------------------------------------------------------------//
//Функция для поиска файла в выбранной директории. на вход имя и путь где искать
bool findFileInDir(const std::string& fileName, const std::filesystem::path& dirPath) {
//в цикле перебираются все ДИРЕКТОРИИ в выбранной dirPath.
    for (const auto &entry : std::filesystem::directory_iterator{dirPath}) {

        std::cout << " Checking " <<entry.path().string() << "\n";
// если это не директория, то проверяется на соответствие заданному имени файла, может это и есть искомый файл.
//и возвращает true, если файл найден.
        if (!is_directory(entry)) {
            if (entry.path().filename() == fileName) {
                std::cout << "File path is: " << std::endl;
                std::filesystem::path p = entry.path();
                std::cout << p.make_preferred().string() << std::endl;

                return true;
            }
// Иначе вызываем функцию findFileInSubDir для поиска внутри директории.
// Если результат работы findFileInSubDir является true, то вызывается true здесь.
        } else {
            if (findFileInSubDir(fileName, entry)) {
                return true;
            }
            std::cout << " file is not found in " << entry.path().string() << "\n";
        }
    }
    return false;
}
//----------------------------------------------------------------------//
//Функция для поиска файла в выбранной стартовой директории. на вход имя и путь где искать
//Проверяет правильность пути и перебирает диски. поиском занимается findFileInDir, которая вызывается изнутри.
void fileFinder(const std::string& fileName, std::filesystem::path& startDirPath) {
    int offset = 'a' - 'A';
//Если первый символ строки большая буква диска (D:/), то заменить ее на маленькую за счет оффсета.
    if ( startDirPath.string()[0] >= 'A' && startDirPath.string()[0] <= 'Z' ) {
        std::string p = startDirPath.string();
        p.front() = startDirPath.string()[0] + offset;
        startDirPath = p;
    }
//Если зашедший в функцию путь не является директорией, то выдать сообщение и присвоить диск с:/ как стартовую директорию
    if (!is_directory(startDirPath)) {
        std::cout << startDirPath << " is invalid path." << std::endl;
        startDirPath = "c:\\";
        std::cout << "Default directory is c:\\" << std::endl;
    }
//Если функция findFileInDir нашла файл, то выйти из функции
    if (findFileInDir(fileName, startDirPath)) {
        return;
    }
//Перебор дисков по алфавиту. Создание формирование новой строки формата пути "[drive]:\";
//Если диск равен текущему вошедшему в функцию, то выбрать следующий по списку.
//TODO если выбрать диск следующий после с, то диск с будет пропушен в переборе
    for( char drive = 'd'; drive <= 'z'; ++drive ) {
        std::filesystem::path nextDir = std::string(1, drive) + ":\\";
        if (drive == startDirPath.string()[0] ) {
            nextDir = std::string(1, ++drive) + ":\\";
        }
//Проверка если это директория, не вышли за диск "z:\", то вызвать функцию findFileInDir для поиска в директории.
// если найден файл, то выйти из функции. Если не найден, то выведет сообщение, что не найден файл.
        if (is_directory(nextDir)) {
            if (findFileInDir(fileName, nextDir)) {
                return;
            }
        }
    }
    std::cout << "File not found."<< "\n";
}
