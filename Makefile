CXX = g++
CXXFLAGS = -std=c++20 -g -O0 -Wall -Wextra

# The server needs Winsock on Windows; the variable stays empty elsewhere.
ifeq ($(OS),Windows_NT)
  SOCKETLIBS = -lws2_32
endif

all : main

test: tests/test.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/test.cpp AdjList.o Algorithms.o catchmain.o -o test

test_alg: tests/test_alg.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/test_alg.cpp AdjList.o Algorithms.o catchmain.o -o test_alg

testBFS: tests/testBFS.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/testBFS.cpp AdjList.o Algorithms.o catchmain.o -o testBFS

testRouting: tests/testRouting.cpp AdjList.o Algorithms.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/testRouting.cpp AdjList.o Algorithms.o catchmain.o -o testRouting

testPool: tests/testPool.cpp ThreadPool.o catchmain.o
	$(CXX) $(CXXFLAGS) tests/testPool.cpp ThreadPool.o catchmain.o -o testPool -pthread

main: main.o AdjList.o Algorithms.o
	$(CXX) $(CXXFLAGS) main.o AdjList.o Algorithms.o -o main

server: server.cpp AdjList.o Algorithms.o ThreadPool.o
	$(CXX) $(CXXFLAGS) server.cpp AdjList.o Algorithms.o ThreadPool.o -o server -pthread $(SOCKETLIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

AdjList.o: AdjList.h AdjList.cpp
	$(CXX) $(CXXFLAGS) -c AdjList.cpp

Algorithms.o: Algorithms.cpp
	$(CXX) $(CXXFLAGS) -c Algorithms.cpp

ThreadPool.o: ThreadPool.h ThreadPool.cpp
	$(CXX) $(CXXFLAGS) -c ThreadPool.cpp

catchmain.o: catch/catchmain.cpp
	$(CXX) $(CXXFLAGS) -c catch/catchmain.cpp

.PHONY: clean
clean:
	rm -f *.o *.exe main server test test_alg testBFS testRouting testPool
