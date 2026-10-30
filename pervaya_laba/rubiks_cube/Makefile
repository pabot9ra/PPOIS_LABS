CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
UT_INC = -I/opt/homebrew/include
UT_LIB = -L/opt/homebrew/lib -lUnitTest++

SRC = face.cpp cube.cpp
HDR = face.h cube.h

all: prog tests

prog: main.cpp menu.cpp menu.h $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o prog main.cpp menu.cpp $(SRC)

tests: tests.cpp $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(UT_INC) -o tests tests.cpp $(SRC) $(UT_LIB)

test: tests
	./tests

clean:
	rm -rf prog tests *.dSYM

.PHONY: all test clean
