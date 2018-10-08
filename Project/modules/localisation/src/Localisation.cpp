/*
 * This should take the Ultrasonic readings and the current position and populate an occupancy map of the surroundings
 * Inputs:  Ultrasonic sensor readings
 *          Position
 * Outputs: Occupancy map
 *
 */

#include "Localisation.hpp"

// X Y Current position in world coordinates
double Localisation::w_Tank_Position[2] = {0};
// Rotation about the w_Tank_Position in the anticlockwise direction (radians)
double Localisation::w_Tank_Rotation = 0;
// X Y Goal position in world coordinates
double Localisation::w_Goal_Position[2] = {0};


int Grid::m            = Grid_m;       // dimensions that are mxn
int Grid::n            = Grid::m;      //
int Grid::start_row    = Grid::m;      // assume starting position is in the middle bottom (for robot)
int Grid::start_column = Grid::m / 2;  //

double Grid::area      = 1.5;    // this is the dimensions of the grid in Y*Y meters
double Grid::gridspace = 0.1;    // this is the dimensions of the gridsquares in Y*Y meters
double Grid::map[m][n] = {0.5};  // m x n matrix that serves as the occupancy map


void Localisation_init() {}

int Localisation_main() {

    // what cell is the tank in, note that the x is forward and y is sideways, 0,0 is defined to be the bottom right
    //     corner -return a x and y for what cell we are looking at stuff in
    int tank_cell_m = floor(Localisation::w_Tank_Position[0] / Grid::gridspace);
    int tank_cell_n = floor(Localisation::w_Tank_Position[1] / Grid::gridspace);

    // these are some arrays to store "world" information based on sensors readings
    double sen_hori[4];
    double sen_vert[4];
    double sen_theta[4];

    // now the matrixes have been used instead, can use a loop to calculate these, probs a bit neater
    // what cell are we looking at stuff in- we need to convert the dist and angle to y,x coorinates

    // Front sonic (array pos=0)
    sen_theta[0] = Localisation::w_Tank_Rotation;
    sen_hori[0]  = Ultrasonic::Detection_distances[0] * cos(sen_theta[0]);
    sen_vert[0]  = Ultrasonic::Detection_distances[0] * sin(sen_theta[0]);

    // Right sonic (array pos=1)
    sen_theta[1] = Localisation::w_Tank_Rotation + M_PI + M_PI_2;
    sen_hori[1]  = Ultrasonic::Detection_distances[1] * cos(sen_theta[1]);
    sen_vert[1]  = Ultrasonic::Detection_distances[1] * sin(sen_theta[1]);

    // Back sonic (array pos=2)
    sen_theta[2] = Localisation::w_Tank_Rotation + M_PI;
    sen_hori[2]  = Ultrasonic::Detection_distances[2] * cos(sen_theta[2]);
    sen_vert[2]  = Ultrasonic::Detection_distances[2] * sin(sen_theta[2]);

    // Left sonic (array pos=3)
    sen_theta[3] = Localisation::w_Tank_Rotation + M_PI_2;
    sen_hori[3]  = Ultrasonic::Detection_distances[3] * cos(sen_theta[3]);
    sen_vert[3]  = Ultrasonic::Detection_distances[3] * sin(sen_theta[3]);

    // convert all of these to grid spaces and within here update occupancy map
    for (int i = 0; i < SENSORS; i++) {
        int object_cell_m = tank_cell_m + floor(sen_hori[i] / Grid::gridspace);
        int object_cell_n = tank_cell_n + floor(sen_vert[i] / Grid::gridspace);

        // startign closest to the tank, look at all the grid squares in the way by converting these points to a
        // straight line
        // y=mx+b for now we are using Bresham's algorithim
        breshams_alg(i, &sen_hori, &sen_vert);
        std::vector<std::pair<int, int>>::const_iterator curr_cell_list = Grid::cell_list.begin();

        while (!curr_cell_list.empty()) {
            // this will give a list of grid ms and ns that need to be looked at,starting from
            // closest to the tank
            // need to access the list pairs seperately and save as cella(m) and cellb(n)

            // use pythagoras to calculate distance between tank and the cell we are looking at
            double cell_dist =
                sqrt(((cella - tank_cell_m) * Grid::gridspace) ^ 2 + ((cellb - tank_cell_n) * Grid::gridspace) ^ 2);

            // function to calculate probability
            probability(curr_cell_list->first, curr_cell_list->second, cell_dist, Ultrasonic::Detection_distances[i]);

            // Remove the element from the list
            curr_cell_list.erase(curr_cell_list.begin());
        }
    }
    return 0;
}

void probability(int cell_column, int cell_row, double cell_dist, double obj_dist) {

    // use a straight line function to start however i believe tanh would work nicely
    double prob = (0.2 * cell_dist) / obj_dist - 0.1;
    Grid::map[m][n] += prob;
}

void breshams_alg(int i, double sen_hori, double sen_vert) {
    double dx = sen_hori[i] - Localisation::w_Tank_Position[0];  // dx=x2-x1
    double dy = sen_vert[i] - Localisation::w_Tank_Position[1];  // dy=y2-y1
    double de = abs((dy / dx));                                  // de=abs(dy/dx)
    double e  = 0;
    double y  = Localisation::w_Tank_Position[1];

    for (double x = Localisation::w_Tank_Position[0]; x <= sen_hori[i]; Grid::gridspace) {
        Grid::cell_list.emplace_back(std::make_pair(floor(x / Grid::gridspace), floor(y / Grid::gridspace)));
        e += de;
        if (e >= 0.5) {
            y += (signbit(dy) == 1 : 1 ? -1) * Grid::gridspace;
            e -= Grid::gridspace;
        }
    }
}


// // 2D Map
// // This is a crude method of creating a 2D localisation map
// // Given some fixed workspace, divide it into a square grid section
// // For this implementation 1.5m x 1.5m with a grid spacing of 0.100- based off a tank diagonal size of 440mm
// double area = 1.5;

// // }

// // // 2D Map
// // // This is a crude method of creating a 2D localisation map
// // // Given some fixed workspace, divide it into a square grid section
// // // For this implementation 1.5m x 1.5m with a grid spacing of 0.100- based off a tank diagonal size of 440mm
// //     double area=1.5;

// // //note this will mean that you need 4 vacant grid spaces to pass through
// // //this may need to be changed based on run time
// double gridspace = 0.1;

// // // map[15][15]
// // //for a grid tht is x[m][n]
// int m = 15;
// int n = m;
// // //this is a square :)
// // // create m by n matrix- note this needs to be a double as we are looking at probabilities- could change if this
// is
// // too slow
// double map[m][n] = {0.5};


// // //assume starting position is in the middle bottom
// int start_row    = m;
// int start_column = m / 2;

// // map[x][y] = transform_us_world(us_id, get_dist(us_id));

// // int transfor_us_world(int us_id, double dist){
// //     	using kinematics for the specific us pos
// //		using the current odometry position of the tank
// //		transform the distance of the us_id to world coordinates
// //		populate the cell closest to that x,y pos
// //		return cell id
// // }

// // TODO this doesnt handle a changing environment
// // need to account for objects that were on the map that have now been moved
// // if we had an object beentween us and the new object, then clearly it's not there anymore

// // TODO this doesnt account for objects seen..
// // maybe there needs to be a second layer to the map that contains visually seen objects
// // or another map

// // Occupancy mapping - apparently the name for doing ^^
// // Suppose we build a grid fixed to the world.
// // We start at some position withing the grid (probably initilise to some rough estimation of the center)
// // Now we fill the grid with all equally weighted probabilities of an object being in that location
// // Lets call this 0.5
// // When we detect an object at a distance D on for instance or left hand side.
// // Take the current grid position of the tank
// // Figure out the grid position of the object (its probably somewhere relative to the known position)
// // Now for each cell between us and the object position, calculate a new probability for that cell to be occupied
// // Essentially make a function that takes these inputs:
// // - Distance between the robot and the obstacle / the cell
// // - Previous probability of the cell
// // And outputs an updated probability
// // This function might work over some exponential of the distance or something

// // This grid spacing is the problem... do you make the grid the size of the radius of the robot
// // which means for path planning you only need a single cell space to make it through,
// // or do you make the size of the grid some smaller amount and need to do some harder path planning
