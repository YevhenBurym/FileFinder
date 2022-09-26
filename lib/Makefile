all: FileFinder 
	ar rc libFileFinder.a Filefinder.o
	
FileFinder: FileFinder.cpp
	g++ -std=c++17 -c FileFinder.cpp -o FileFinder.o -lstdc++fs

clean:
	rm  *.o