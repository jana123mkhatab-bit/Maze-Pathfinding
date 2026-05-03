#include "Maze.h"
#include "MyVector.h"
#include <iostream>
#include <algorithm>

using namespace std;

bool isValid(int r, int c, const MazeData& maze) {
    return r >= 0 && r < maze.FULL_R && c >= 0 && c < maze.FULL_C;
}

void generateDFS(int r, int c, MazeData& maze) {
    maze.grid[r][c] = PATH;

    MyVector<int> directions; //it controls the indexes of the direction
    directions.push_back(0);
    directions.push_back(1);
    directions.push_back(2);
    directions.push_back(3);

    // Simple shuffle
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int idx = 0; idx < 4; ++idx) {
        int i = directions[idx];
        int nr = r + DR_GEN[i];
        int nc = c + DC_GEN[i];
        int wall_r = r + DR_SOLVE[i];
        int wall_c = c + DC_SOLVE[i];

        if (isValid(nr, nc, maze) && maze.grid[nr][nc] == WALL) {
            maze.grid[wall_r][wall_c] = PATH;
            generateDFS(nr, nc, maze);
        }
    }
}

MazeData generateMaze(int R, int C, const Point& start, const Point& goal) {
    MazeData maze;
    maze.R = R;
    maze.C = C;
    maze.startPoint = start;
    maze.goalPoint = goal;

    maze.FULL_R = 2 * R + 1;
    maze.FULL_C = 2 * C + 1;

    // Initialize grid
    maze.grid = MyVector<MyVector<int>>(maze.FULL_R);
    maze.weights = MyVector<MyVector<int>>(maze.FULL_R);

    for (int r = 0; r < maze.FULL_R; ++r) {
        maze.grid[r] = MyVector<int>(maze.FULL_C);
        maze.weights[r] = MyVector<int>(maze.FULL_C);
        for (int c = 0; c < maze.FULL_C; ++c) {
            maze.grid[r][c] = WALL;
            maze.weights[r][c] = 0;
        }
    }

    srand(time(0));
    int start_r = 2 * (rand() % R) + 1;
    int start_c = 2 * (rand() % C) + 1;

    generateDFS(start_r, start_c, maze);

    for (int r = 0; r < maze.FULL_R; ++r) {
        for (int c = 0; c < maze.FULL_C; ++c) {
            if (maze.grid[r][c] != WALL) {
                maze.weights[r][c] = (rand() % 9) + 1;
            }
        }
    }

    int adjStartR = 2 * maze.startPoint.r + 1;
    int adjStartC = 2 * maze.startPoint.c + 1;
    int adjGoalR = 2 * maze.goalPoint.r + 1;
    int adjGoalC = 2 * maze.goalPoint.c + 1;

    maze.startPoint = {adjStartR, adjStartC};
    maze.goalPoint = {adjGoalR, adjGoalC};

    if (isValid(adjStartR, adjStartC, maze) && maze.grid[adjStartR][adjStartC] != WALL) {
        maze.grid[adjStartR][adjStartC] = START;
        maze.weights[adjStartR][adjStartC] = 1;
    }
    if (isValid(adjGoalR, adjGoalC, maze) && maze.grid[adjGoalR][adjGoalC] != WALL) {
        maze.grid[adjGoalR][adjGoalC] = GOAL;
    }

    return maze;
}
