#ifndef MAZEALGORITHMS_H
#define MAZEALGORITHMS_H

#include "Maze.h"
#include "MyVector.h"


// we used struct here becuase it's just a plain container that holds
// the result of data
// we do not need class as we don't use any methods or need settters/ getters
struct AlgorithmResult {

    bool success;
    int totalCost;
    int nodesExpanded;
    double timeTakenMs;
    
    // it's a pic of the maze after it's solved,
    // that is way we started by initializing it
    // with the actual maze grid
    // then we return the grid but this time with the path being found
     MyVector<MyVector<int>> solutionGrid; // it stores every cell in the maze
    
    
    // it stores the steps we took to solve the maze from s to g
    MyVector<Point> path; // only stores the path
    
};

// every algo that solves the maze, is
// a function that returns AlgorithmResult (the final result of the operation for comparison)
// the variable that is returned is result
// we initialze result at the beginning of each function in .cpp
AlgorithmResult solveDijkstra(const MazeData& maze);
AlgorithmResult solveBucket(const MazeData& maze);
AlgorithmResult solveAStar(const MazeData& maze);

#endif // MAZEALGORITHMS_H
