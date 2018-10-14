#ifndef SIMPLE_PATH_PLANNER_H_
#define SIMPLE_PATH_PLANNER_H_

#include "main.hpp"

#define Map_m 15
#define Map_n 15

class Maze {
public:
    static int map[Map_m][Map_n];
};

// Creating a shortcut for int, int pair type
// typedef std::pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
// typedef std::pair<double, std::pair<int, int>> pPair;

// A structure to hold the neccesary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

extern void SimplePathPlanner_init();
extern int SimplePathPlanner();
extern void Flatten_Map(double threshold);
extern bool isValid(int row, int col);
extern bool isUnBlocked(int grid[][Map_n], int row, int col);
extern bool isDestination(int row, int col, std::pair<int, int> dest);
extern double calculateHValue(int row, int col, std::pair<int, int> dest);
extern void tracePath(cell cellDetails[][Map_n], std::pair<int, int> dest);
extern void aStarSearch(int grid[][Map_n], std::pair<int, int> src, std::pair<int, int> dest);

#endif /* SIMPLE_PATH_PLANNER_H_ */
