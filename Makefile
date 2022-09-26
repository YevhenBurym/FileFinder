all: main
	g++ -o FileFinder.exe main.o -L./lib -lFileFinder -lstdc++fs
main: main.cpp
	g++ -c -std=c++17 main.cpp -o main.o
clean:
	rm  *.o *.exe

