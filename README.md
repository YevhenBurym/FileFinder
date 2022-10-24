# File Finder
This is a program to search for a file in the system. Multithreading is used to speed up the search.
The file search algorithm is implemented as a static library `libFileFinder.a`. The application can search for a file in the system or in a particular directory.
You can also search until the first match or all matches.
## Used
- cpp 17
- multithreading
- std::filesystem

## How to run this project
 - clone branch `main` with `https://github.com/YevhenBurym/FileFinder.git` or fork it and then clone it from your forked repo.
 - The static library `libFileFinder.a` can be built using a `/lib/Makefile`. You can also use an already compiled library `/lib/libFileFinder.a`.
 - The project can be built using a `Makefile` in the project's root directory.
 - You can also run the compiled program through the executable file `FileFinder.exe` in the root directory of the project.
## Examples
 - if you want to search the entire system for the first match: `FileFinder.exe <file_name>`.  
 - if you want to search in the directory for the first match: `FileFinder.exe <file_name> <directory_path>`.
 - Add the `-all` parameter in the end to search for all matches (`FileFinder.exe <file_name> -all` or `FileFinder.exe <file_name> <directory_path> -all`).