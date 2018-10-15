/*
 * This should take the occupancy map and plan a path to a given goal
 * Inputs:  Goal of robot relative to robot as vector
            Any obstacle for the robot to avoid as vector
            Kinematics
 * Outputs: Path to follow as some sort of vector function
 */

// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/


#include "SimplePathPlanner.hpp"

int Maze::map[Map_m][Map_n] = {0};

void SimplePathPlanner_init() {}

// Lets make out grid into a B&W matrix
void Flatten_Map(double threshold) {
    for (int rows = 0; rows < Map_m; rows++) {
        for (int cols = 0; cols < Map_n; cols++) {
            Maze::map[rows][cols] = Grid::map[rows][cols] > threshold ? 0 : 1;
        }
    }
}


// Driver program to test above function
int SimplePathPlanner(Start, End) {
    /* Description of the Grid-
     1--> The cell is not blocked
     0--> The cell is blocked    */

    Flatten_Map(0.7);

    // Calculate which cell we are in
    std::pair<int, int> src = std::make_pair(std::floor(Localisation::w_Tank_Position[0] / Grid::gridspace),
                                             std::floor(Localisation::w_Tank_Position[1] / Grid::gridspace));

    // Calculate which cell we want to finish in
    std::pair<int, int> dest = std::make_pair(std::floor(Localisation::w_Goal_Position[0] / Grid::gridspace),
                                              std::floor(Localisation::w_Goal_Position[1] / Grid::gridspace));

    aStarSearch(Maze::map, src, dest);

    return (0);
}

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col) {
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < Map_m) && (col >= 0) && (col < Map_n);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[][Map_n], int row, int col) {
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, std::pair<int, int> dest) {
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, std::pair<int, int> dest) {
    // Return using the distance formula
    return ((double) std::sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell cellDetails[][Map_n], std::pair<int, int> dest) {
    std::cout << "The Path is " << std::endl;
    int row = dest.first;
    int col = dest.second;

    std::stack<std::pair<int, int>> Path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        Path.push(std::make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row          = temp_row;
        col          = temp_col;
    }

    PathPlanner pplanner;

    double Forward  = 0;
    double Rotation = 0;

    double Prev_Position[2] = {Localisation::w_Tank_Position[0], Localisation::w_Tank_Position[1]};
    double Next_Position[2] = {0};

    double Previous_Rotation = Localisation::w_Tank_Rotation;
    double Next_Rotation     = Localisation::w_Tank_Rotation;

    Path.push(std::make_pair(row, col));
    std::pair<int, int> p = Path.top();
    Path.pop();

    while (!Path.empty()) {

        std::pair<int, int> p = Path.top();
        Path.pop();
        std::cout << "-> (" << p.first << ", " << p.second << ")" << std::endl;

        // Convert the grid cell to a world coordinate
        Next_Position[0] = p.first * Grid::gridspace + Grid::gridspace / 2.0;
        Next_Position[1] = p.second * Grid::gridspace + Grid::gridspace / 2.0;

        // Find the vector from my previous position to my next position
        Next_Position[0] = Next_Position[0] - Prev_Position[0];
        Next_Position[1] = Next_Position[1] - Prev_Position[1];
        // Calculate the rotation difference
        Next_Rotation = std::atan2(t_Goal_Position[1], t_Goal_Position[0]);

        // Convert into a forward and rotation command
        Forward  = std::sqrt(std::pow(t_Goal_Position[0], 2) + std::pow(t_Goal_Position[1], 2));
        Rotation = Previous_Rotation - Next_Rotation;

        // Add the polar vector to my list
        // Rotation is relative to the vehicle, not the world
        pplanner.emplace_path(Forward, Rotation);
    }

    // Remove that last member and replace it with the final goal
    std::vector<std::pair<double, double>>::const_iterator ret_vec = pplanner.back();
    pplanner.popback();

    // Convert the grid cell to a world coordinate
    Next_Position[0] = Localisation::w_Goal_Postition[0];
    Next_Position[1] = Localisation::w_Goal_Postition[1];

    // Find the vector from my previous position to my next position
    Next_Position[0] = Next_Position[0] - Prev_Position[0];
    Next_Position[1] = Next_Position[1] - Prev_Position[1];
    // Calculate the rotation difference
    Next_Rotation = std::atan2(t_Goal_Position[1], t_Goal_Position[0]);

    // Convert into a forward and rotation command
    Forward  = std::sqrt(std::pow(t_Goal_Position[0], 2) + std::pow(t_Goal_Position[1], 2));
    Rotation = Previous_Rotation - Next_Rotation;

    // Add the polar vector to my list
    // Rotation is relative to the vehicle, not the world
    pplanner.emplace_path(Forward, Rotation);
    return;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(int grid[][Map_n], std::pair<int, int> src, std::pair<int, int> dest) {
    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        std::cout << "Source is invalid" << std::endl;
        return;
    }

    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        std::cout << "Destination is invalid" << std::endl;
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.first, src.second) == false || isUnBlocked(grid, dest.first, dest.second) == false) {
        std::cout << "Source or the destination is blocked" << std::endl;
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest) == true) {
        std::cout << "We are already at the destination" << std::endl;
        return;
    }

    // Create a closed list and initialise it to false which means
    // that no cell has been included yet
    // This closed list is implemented as a boolean 2D array
    bool closedList[Map_m][Map_n];
    memset(closedList, false, sizeof(closedList));

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[Map_m][Map_n];

    int i, j;

    for (i = 0; i < Map_m; i++) {
        for (j = 0; j < Map_n; j++) {
            cellDetails[i][j].f        = FLT_MAX;
            cellDetails[i][j].g        = FLT_MAX;
            cellDetails[i][j].h        = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f        = 0.0;
    cellDetails[i][j].g        = 0.0;
    cellDetails[i][j].h        = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implenented as a set of pair of pair.*/
    std::set<std::pair<double, std::pair<int, int>>> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        std::pair<double, std::pair<int, int>> p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i                = p.second.first;
        j                = p.second.second;
        closedList[i][j] = true;

        /*
         Generating all the 8 successor of this cell

             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \
                /   |  \
             S.W    S   S.E

         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West           (i, j-1)
         N.E--> North-East  (i-1, j+1)
         N.W--> North-West  (i-1, j-1)
         S.E--> South-East  (i+1, j+1)
         S.W--> South-West  (i+1, j-1)*/

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j] == false && isUnBlocked(grid, i - 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f        = fNew;
                    cellDetails[i - 1][j].g        = gNew;
                    cellDetails[i - 1][j].h        = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j] == false && isUnBlocked(grid, i + 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f        = fNew;
                    cellDetails[i + 1][j].g        = gNew;
                    cellDetails[i + 1][j].h        = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j + 1] == false && isUnBlocked(grid, i, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j + 1].f        = fNew;
                    cellDetails[i][j + 1].g        = gNew;
                    cellDetails[i][j + 1].h        = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j - 1] == false && isUnBlocked(grid, i, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j - 1)));

                    // Update the details of this cell
                    cellDetails[i][j - 1].f        = fNew;
                    cellDetails[i][j - 1].g        = gNew;
                    cellDetails[i][j - 1].h        = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- 5th Successor (North-East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j + 1] == false && isUnBlocked(grid, i - 1, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j + 1].f == FLT_MAX || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j + 1].f        = fNew;
                    cellDetails[i - 1][j + 1].g        = gNew;
                    cellDetails[i - 1][j + 1].h        = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 6th Successor (North-West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j - 1] == false && isUnBlocked(grid, i - 1, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j - 1].f == FLT_MAX || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j - 1)));
                    // Update the details of this cell
                    cellDetails[i - 1][j - 1].f        = fNew;
                    cellDetails[i - 1][j - 1].g        = gNew;
                    cellDetails[i - 1][j - 1].h        = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- 7th Successor (South-East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j + 1] == false && isUnBlocked(grid, i + 1, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j + 1].f == FLT_MAX || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j + 1].f        = fNew;
                    cellDetails[i + 1][j + 1].g        = gNew;
                    cellDetails[i + 1][j + 1].h        = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 8th Successor (South-West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                std::cout << "The destination cell is found" << std::endl;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j - 1] == false && isUnBlocked(grid, i + 1, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j - 1].f == FLT_MAX || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j - 1].f        = fNew;
                    cellDetails[i + 1][j - 1].g        = gNew;
                    cellDetails[i + 1][j - 1].h        = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destiantion cell. This may happen when the
    // there is no way to destination cell (due to blockages)
    if (foundDest == false) std::cout << "Failed to find the Destination Cell" << std::endl;

    return;
}