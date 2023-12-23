CXX = clang++
CXXFLAGS = -std=c++0x -g -O0 -Wall -Wextra

all : main

test: tests/test.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/test.cpp AdjList.o Algorithms.o catchmain.o -o test

test_alg: tests/test_alg.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/test_alg.cpp AdjList.o Algorithms.o catchmain.o -o test_alg

testBFS: tests/testBFS.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/testBFS.cpp AdjList.o Algorithms.o catchmain.o -o testBFS

main: main.o AdjList.o Algorithms.o
	$(CXX) $(CXXFLAGS) main.o AdjList.o Algorithms.o -o main

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

AdjList.o: AdjList.h AdjList.cpp
	$(CXX) $(CXXFLAGS) -c AdjList.cpp

Algorithms.o: Algorithms.cpp
	$(CXX) $(CXXFLAGS) -c Algorithms.cpp

catchmain.o: catch/catchmain.cpp
	$(CXX) $(CXXFLAGS) -c catch/catchmain.cpp

.PHONY: clean
clean:
	rm -f *.o $(EXENAME) main
