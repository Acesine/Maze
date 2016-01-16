all: MazeRunner
	./MazeRunner

MazeRunner:
	g++ -std=c++11 MazeRunner.cpp -o MazeRunner
