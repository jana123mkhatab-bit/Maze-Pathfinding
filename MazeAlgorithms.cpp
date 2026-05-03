#include "MazeAlgorithms.h"
#include "MyVector.h"
#include "MyPriorityQueue.h"
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;
// Helper Functions
bool isPointValid(int r, int c, const MazeData& maze) {
    return r >= 0 && r < maze.FULL_R && c >= 0 && c < maze.FULL_C;
}
int heuristic(int x, int y, int goalX, int goalY) {
    return abs(goalX - x) + abs(goalY - y);
}


// ========== DIJKSTRA ==========//METHOD
AlgorithmResult solveDijkstra(const MazeData& maze) {
    auto startTime = high_resolution_clock::now();
    AlgorithmResult result;
    result.success = false;
    result.totalCost = INF;
    result.nodesExpanded = 0;
    result.solutionGrid = maze.grid;

    // Initialize arrays using MyVector
    MyVector<MyVector<int>> dist(maze.FULL_R);
    MyVector<MyVector<bool>> visited(maze.FULL_R);
    MyVector<MyVector<int>> parentX(maze.FULL_R);
    MyVector<MyVector<int>> parentY(maze.FULL_R);

    for (int i = 0; i < maze.FULL_R; ++i) {
        dist[i] = MyVector<int>(maze.FULL_C);
        visited[i] = MyVector<bool>(maze.FULL_C);
        parentX[i] = MyVector<int>(maze.FULL_C);
        parentY[i] = MyVector<int>(maze.FULL_C);

        for (int j = 0; j < maze.FULL_C; ++j) {
            dist[i][j] = INT_MAX;
            visited[i][j] = false;
            parentX[i][j] = -1;
            parentY[i][j] = -1;
        }
    }

    struct Node {
        int x, y;
        int cost;

        bool operator<(const Node& other) const {
            return cost > other.cost; // Min-heap
        }
    };

    MyPriorityQueue<Node> pq;


    dist[maze.startPoint.r][maze.startPoint.c] = maze.weights[maze.startPoint.r][maze.startPoint.c];
    //push(value=node)
    //push(x,y,cost)
    pq.push({maze.startPoint.r, maze.startPoint.c, dist[maze.startPoint.r][maze.startPoint.c]});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        //the current cell cordinates that i have
        int x = cur.x, y = cur.y, cost = cur.cost;

        if (visited[x][y]) continue;
        visited[x][y] = true;
        result.nodesExpanded++;

        if (x == maze.goalPoint.r && y == maze.goalPoint.c) {
            result.totalCost = cost;
            result.success = true;

            int cx = maze.goalPoint.r, cy = maze.goalPoint.c;
            while (!(cx == maze.startPoint.r && cy == maze.startPoint.c)) {
                if (result.solutionGrid[cx][cy] != GOAL && result.solutionGrid[cx][cy] != START) {
                    result.solutionGrid[cx][cy] = SHORTEST_PATH;
                }
                result.path.push_back({cx, cy});
                int px = parentX[cx][cy];
                int py = parentY[cx][cy];
                cx = px; cy = py;
            }
            result.path.push_back({maze.startPoint.r, maze.startPoint.c});

            // Reverse the path manually
            MyVector<Point> reversedPath;
            for (int i = result.path.size() - 1; i >= 0; --i) {
                reversedPath.push_back(result.path[i]);
            }
            result.path = reversedPath;

            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!isPointValid(nx, ny, maze) || maze.grid[nx][ny] == WALL) continue;
            if (visited[nx][ny]) continue;

            int newCost = cost + maze.weights[nx][ny];

            if (newCost < dist[nx][ny]) {
                dist[nx][ny] = newCost;
                parentX[nx][ny] = x;
                parentY[nx][ny] = y;
                pq.push({nx, ny, newCost});
            }
        }
    }

    auto endTime = high_resolution_clock::now();
    result.timeTakenMs = duration_cast<duration<double, milli>>(endTime - startTime).count();
    return result;
}

// ========== BUCKET ==========
AlgorithmResult solveBucket(const MazeData& maze) {
    auto startTime = high_resolution_clock::now();
    AlgorithmResult result;
    result.success = false;
    result.totalCost = INF;
    result.nodesExpanded = 0;
    result.solutionGrid = maze.grid;

    MyVector<MyVector<int>> dist(maze.FULL_R);
    MyVector<MyVector<bool>> visited(maze.FULL_R);
    MyVector<MyVector<int>> parentX(maze.FULL_R);
    MyVector<MyVector<int>> parentY(maze.FULL_R);

    for (int i = 0; i < maze.FULL_R; ++i) {
        dist[i] = MyVector<int>(maze.FULL_C);
        visited[i] = MyVector<bool>(maze.FULL_C);
        parentX[i] = MyVector<int>(maze.FULL_C);
        parentY[i] = MyVector<int>(maze.FULL_C);

        for (int j = 0; j < maze.FULL_C; ++j) {
            dist[i][j] = INT_MAX;
            visited[i][j] = false;
            parentX[i][j] = -1;
            parentY[i][j] = -1;
        }
    }

    const int MAX_COST = 10000;
    const int DELTA = 1;

    struct Node {
        int x, y;
        int cost;
    };

    struct Bucket {
        MyVector<Node> nodes;
        int size;
    };

    MyVector<Bucket> buckets;
    for (int i = 0; i < MAX_COST; ++i) {
        Bucket b;
        b.size = 0;
        buckets.push_back(b);
    }

    dist[maze.startPoint.r][maze.startPoint.c] = maze.weights[maze.startPoint.r][maze.startPoint.c];
    int startBucket = dist[maze.startPoint.r][maze.startPoint.c] / DELTA;
    if (startBucket >= MAX_COST) startBucket = MAX_COST - 1;
    if (startBucket < 0) startBucket = 0;

    buckets[startBucket].nodes.push_back({maze.startPoint.r, maze.startPoint.c,
                                          dist[maze.startPoint.r][maze.startPoint.c]});
    buckets[startBucket].size++;

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int b = 0; b < MAX_COST; b++) {
        while (buckets[b].size > 0) {
            Node cur = buckets[b].nodes[--buckets[b].size];
            int x = cur.x, y = cur.y;

            if (visited[x][y]) continue;
            visited[x][y] = true;
            result.nodesExpanded++;

            if (x == maze.goalPoint.r && y == maze.goalPoint.c) {
                result.totalCost = dist[x][y];
                result.success = true;

                int cx = maze.goalPoint.r, cy = maze.goalPoint.c;
                while (!(cx == maze.startPoint.r && cy == maze.startPoint.c)) {
                    if (result.solutionGrid[cx][cy] != GOAL && result.solutionGrid[cx][cy] != START) {
                        result.solutionGrid[cx][cy] = SHORTEST_PATH;
                    }
                    result.path.push_back({cx, cy});
                    int px = parentX[cx][cy];
                    int py = parentY[cx][cy];
                    cx = px; cy = py;
                }
                result.path.push_back({maze.startPoint.r, maze.startPoint.c});

                // Reverse the path manually
                MyVector<Point> reversedPath;
                for (int i = result.path.size() - 1; i >= 0; --i) {
                    reversedPath.push_back(result.path[i]);
                }
                result.path = reversedPath;

                auto endTime = high_resolution_clock::now();
                result.timeTakenMs = duration_cast<duration<double, milli>>(endTime - startTime).count();
                return result;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (!isPointValid(nx, ny, maze) || maze.grid[nx][ny] == WALL) continue;
                if (visited[nx][ny]) continue;

                int newCost = dist[x][y] + maze.weights[nx][ny];

                if (newCost < dist[nx][ny]) {
                    dist[nx][ny] = newCost;
                    parentX[nx][ny] = x;
                    parentY[nx][ny] = y;

                    int newBucket = newCost / DELTA;
                    if (newBucket >= MAX_COST) newBucket = MAX_COST - 1;
                    if (newBucket < 0) newBucket = 0;

                    buckets[newBucket].nodes.push_back({nx, ny, newCost});
                    buckets[newBucket].size++;
                }
            }
        }
    }

    auto endTime = high_resolution_clock::now();
    result.timeTakenMs = duration_cast<duration<double, milli>>(endTime - startTime).count();
    return result;
}

// ========== A* ==========
AlgorithmResult solveAStar(const MazeData& maze) {
    auto startTime = high_resolution_clock::now();
    AlgorithmResult result;
    result.success = false;
    result.totalCost = INF;
    result.nodesExpanded = 0;
    result.solutionGrid = maze.grid;

    MyVector<MyVector<int>> gScore(maze.FULL_R);
    MyVector<MyVector<bool>> closedSet(maze.FULL_R);
    MyVector<MyVector<int>> parentX(maze.FULL_R);
    MyVector<MyVector<int>> parentY(maze.FULL_R);

    for (int i = 0; i < maze.FULL_R; ++i) {
        gScore[i] = MyVector<int>(maze.FULL_C); //cost
        closedSet[i] = MyVector<bool>(maze.FULL_C);
        parentX[i] = MyVector<int>(maze.FULL_C);
        parentY[i] = MyVector<int>(maze.FULL_C);

        for (int j = 0; j < maze.FULL_C; ++j) {
            gScore[i][j] = INT_MAX;
            closedSet[i][j] = false;
            parentX[i][j] = -1;
            parentY[i][j] = -1;
        }
    }

    /*auto heuristic = [&](int x, int y) {
        int dr = maze.goalPoint.r - x;
        int dc = maze.goalPoint.c - y;
        if (dr < 0) dr = -dr;
        if (dc < 0) dc = -dc;
        return dr + dc;
    };*/



    struct Node {
        int x, y;
        int g;
        int f;

        bool operator<(const Node& other) const {
            if (f == other.f) return g > other.g;
            return f > other.f;
        }
    };

    MyPriorityQueue<Node> openPQ;

    gScore[maze.startPoint.r][maze.startPoint.c] = maze.weights[maze.startPoint.r][maze.startPoint.c];
    int h0 = heuristic(maze.startPoint.r, maze.startPoint.c, maze.goalPoint.r, maze.goalPoint.c);
    openPQ.push({maze.startPoint.r, maze.startPoint.c,
                 gScore[maze.startPoint.r][maze.startPoint.c],
                 gScore[maze.startPoint.r][maze.startPoint.c] + h0});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!openPQ.empty()) {
        Node cur = openPQ.top();
        openPQ.pop();

        int x = cur.x, y = cur.y, g = cur.g;

        if (g > gScore[x][y]) continue;
        if (closedSet[x][y]) continue;
        closedSet[x][y] = true;
        result.nodesExpanded++;

        if (x == maze.goalPoint.r && y == maze.goalPoint.c) {
            result.totalCost = g;
            result.success = true;

            int cx = maze.goalPoint.r, cy = maze.goalPoint.c;
            while (!(cx == maze.startPoint.r && cy == maze.startPoint.c)) {
                if (result.solutionGrid[cx][cy] != GOAL && result.solutionGrid[cx][cy] != START) {
                    result.solutionGrid[cx][cy] = SHORTEST_PATH;
                }
                result.path.push_back({cx, cy});
                int px = parentX[cx][cy];
                int py = parentY[cx][cy];
                cx = px; cy = py;
            }
            result.path.push_back({maze.startPoint.r, maze.startPoint.c});

            // Reverse the path manually
            MyVector<Point> reversedPath;
            for (int i = result.path.size() - 1; i >= 0; --i) {
                reversedPath.push_back(result.path[i]);
            }
            result.path = reversedPath;

            auto endTime = high_resolution_clock::now();
            result.timeTakenMs = duration_cast<duration<double, milli>>(endTime - startTime).count();
            return result;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!isPointValid(nx, ny, maze) || maze.grid[nx][ny] == WALL) continue;
            if (closedSet[nx][ny]) continue;

            //totalcost
            int tentativeG = g + maze.weights[nx][ny];

            if (tentativeG < gScore[nx][ny]) {
                gScore[nx][ny] = tentativeG;
                parentX[nx][ny] = x;
                parentY[nx][ny] = y;
                int h = heuristic(nx, ny,  maze.goalPoint.r,maze.goalPoint.c);
                int f = tentativeG + h;
                openPQ.push({nx, ny, tentativeG, f});
            }
        }
    }

    auto endTime = high_resolution_clock::now();
    result.timeTakenMs = duration_cast<duration<double, milli>>(endTime - startTime).count();
    return result;
}
