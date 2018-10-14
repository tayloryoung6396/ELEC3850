#ifndef SIMPLE_PATH_PLANNER_H_
#define SIMPLE_PATH_PLANNER_H_

#include "main.hpp"

class Maze {
public:
    static int map[Grid::m][Grid::n];
}

// Creating a shortcut for int, int pair type
typedef std::pair<int, int>
    Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef std::pair<double, std::pair<int, int>> pPair;

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
extern bool isUnBlocked(int grid[][Grid::n], int row, int col);
extern bool isDestination(int row, int col, Pair dest);
extern double calculateHValue(int row, int col, Pair dest);
extern void tracePath(cell cellDetails[][Grid::n], Pair dest);
extern void aStarSearch(int grid[][Grid::n], Pair src, Pair dest);

#endif /* SIMPLE_PATH_PLANNER_H_ */