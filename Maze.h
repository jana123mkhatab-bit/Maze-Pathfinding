#ifndef MAZE_H
#define MAZE_H

#include "MyVector.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

// --- Constants ---
enum CellType {
    WALL = 0,
    PATH = 1,
    START = 2,
    GOAL = 3,
    SHORTEST_PATH = 4
};

// Movement for DFS generation (step of 2)
const int DR_GEN[] = {-2, 2, 0, 0};
const int DC_GEN[] = {0, 0, -2, 2};

// Movement for Dijkstra solving (step of 1)
const int DR_SOLVE[] = {-1, 1, 0, 0};
const int DC_SOLVE[] = {0, 0, -1, 1};

const int INF = 1000000000;

// --- Data Structures ---
struct Point {
    int r, c;

    bool operator<(const Point& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }

    bool operator==(const Point& other) const {
        return r == other.r && c == other.c;
    }
};

struct MazeData {
    int R, C;
    int FULL_R, FULL_C;
    MyVector<MyVector<int>> grid;
    MyVector<MyVector<int>> weights;
    Point startPoint;
    Point goalPoint;

    MazeData() : R(15), C(15), FULL_R(31), FULL_C(31) {}
};

// Utility Functions
bool isValid(int r, int c, const MazeData& maze);
void generateDFS(int r, int c, MazeData& maze);
MazeData generateMaze(int R, int C, const Point& start, const Point& goal);

#endif // MAZE_H
