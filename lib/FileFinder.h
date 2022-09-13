#pragma once

#include <string>
#include <filesystem>

bool findFileInSubDir(const std::string& fileName, const std::filesystem::directory_entry& directory);
bool findFileInDir(const std::string& fileName, const std::filesystem::path& dirPath);
void fileFinder(const std::string& fileName, std::filesystem::path& startDirPath);
void findFileInDirectory(const std::string& fileName, std::filesystem::path& startDirPath);
void findFile(const std::string& fileName);
