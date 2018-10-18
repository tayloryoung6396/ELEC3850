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
double Localisation::w_Tank_Rotation    =  0;
// X Y Goal position in world coordinates
double Localisation::w_Goal_Position[2] = {0};


const int Grid::m            = Grid_m;   // dimensions that are mxn
const int Grid::n            = Grid::m;  //
const int Grid::start_row    = 7;        // assume starting position is in the middle bottom (for robot)
const int Grid::start_column = 7;        //

double Grid::area                  = 1.5;  // this is the dimensions of the grid in Y*Y meters
double Grid::gridspace             = 0.1;  // this is the dimensions of the gridsquares in Y*Y meters
double Grid::map[Grid::m][Grid::n] = {0};  // m x n matrix that serves as the occupancy map

std::vector<std::pair<int, int>> Grid::cell_list;

void Localisation_init() {
    for (int i = 0; i < Grid::m; i++) {
        for (int j = 0; j < Grid::n; j++) {
            Grid::map[i][j] = 0.5;
        }
    }
}

int Localisation_main() {

    std::cout << "Start row " << (int) Grid::start_row << " Start column " << (int) Grid::start_column << std::endl;

    // what cell is the tank in, note that the x is forward and y is sideways, 0,0 is defined to be the bottom right
    //     corner -return a x and y for what cell we are looking at stuff in
    int tank_cell_m = std::floor(Localisation::w_Tank_Position[0] / Grid::gridspace) + Grid::start_row;
    int tank_cell_n = std::floor(Localisation::w_Tank_Position[1] / Grid::gridspace) + Grid::start_column;

    int tank_cell_m = std::floor(Localisation::w_Tank_Position[0] / Grid::gridspace)+Grid::start_row ;
    int tank_cell_n = std::floor(Localisation::w_Tank_Position[1] / Grid::gridspace)+Grid::start_column;
<<<<<<< HEAD
    std::cout << "tank possi m (down) " << tank_cell_m << std::endl;
    std::cout << "tank possi n (across) " << tank_cell_n << std::endl;

=======
>>>>>>> test_branch_alex

    // these are some arrays to store "world" information based on sensors readings
    double sen_hori[4]  = {0};
    double sen_vert[4]  = {0};
    double sen_theta[4] = {0};

    // now the matrixes have been used instead, can use a loop to calculate these, probs a bit neater
    // what cell are we looking at stuff in- we need to convert the dist and angle to y,x coorinates

    // Front sonic (array pos=0)
    sen_theta[0] = Localisation::w_Tank_Rotation;
    sen_hori[0]  = Ultrasonic::Detection_distances[0] * std::cos(sen_theta[0]);
    sen_vert[0]  = Ultrasonic::Detection_distances[0] * std::sin(sen_theta[0]);
    std::cout << "Sensor rotation \t" << sen_theta[0] << " Sensor hori \t" << sen_hori[0] << " Sensor vert \t"
              << sen_vert[0] << std::endl;

    // Right sonic (array pos=1)
    sen_theta[1] = Localisation::w_Tank_Rotation + M_PI + M_PI_2;
    sen_hori[1]  = Ultrasonic::Detection_distances[1] * std::cos(sen_theta[1]);
    sen_vert[1]  = Ultrasonic::Detection_distances[1] * std::sin(sen_theta[1]);
    std::cout << "Sensor rotation \t" << sen_theta[1] << " Sensor hori \t" << sen_hori[1] << " Sensor vert \t"
              << sen_vert[1] << std::endl;

    // Back sonic (array pos=2)
    sen_theta[2] = Localisation::w_Tank_Rotation + M_PI;
    sen_hori[2]  = Ultrasonic::Detection_distances[2] * std::cos(sen_theta[2]);
    sen_vert[2]  = Ultrasonic::Detection_distances[2] * std::sin(sen_theta[2]);
    std::cout << "Sensor rotation \t" << sen_theta[2] << " Sensor hori \t" << sen_hori[2] << " Sensor vert \t"
              << sen_vert[2] << std::endl;

    // Left sonic (array pos=3)
    sen_theta[3] = Localisation::w_Tank_Rotation + M_PI_2;
    sen_hori[3]  = Ultrasonic::Detection_distances[3] * std::cos(sen_theta[3]);
    sen_vert[3]  = Ultrasonic::Detection_distances[3] * std::sin(sen_theta[3]);
    std::cout << "Sensor rotation \t" << sen_theta[3] << " Sensor hori \t" << sen_hori[3] << " Sensor vert \t"
              << sen_vert[3] << std::endl;

<<<<<<< HEAD

    //print some values for sensor meth
    for(int i=0; i<SENSORS; i++)
    {
        std::cout << "Sensor position" << i << "="<<  sen_theta[i]<< std::endl;
        std::cout << "Sensor position" << i << "="<<  sen_hori[i]<< std::endl;
        std::cout << "Sensor position" << i << "="<<  sen_vert[i]<< std::endl;
    }

=======
>>>>>>> test_branch_alex
    // convert all of these to grid spaces and within here update occupancy map
    for (int i = 0; i < SENSORS; i++) {

        std::cout << "Sensor " << i << std::endl;

        int object_cell_m = tank_cell_m + std::floor(sen_hori[i] / Grid::gridspace);
        int object_cell_n = tank_cell_n + std::floor(sen_vert[i] / Grid::gridspace);

        std::cout << "Cell " << object_cell_m << " " << object_cell_n << std::endl;

        // startign closest to the tank, look at all the grid squares in the way by converting these points to a
        // straight line
        // y=mx+b for now we are using Bresham's algorithim
        std::cout << "Calculating list of cells" << std::endl;

        breshams_alg(i, sen_hori, sen_vert);

        std::cout << "Fininished calculating list" << std::endl;

        while (!Grid::cell_list.empty()) {

            std::vector<std::pair<int, int>>::const_iterator curr_cell_list = Grid::cell_list.begin();

            // this will give a list of grid ms and ns that need to be looked at,starting from
            // closest to the tank
            // need to access the list pairs seperately and save as cella(m) and cellb(n)

            // use pythagoras to calculate distance between tank and the cell we are looking at
            double cell_dist = std::sqrt(std::pow(((curr_cell_list->first - tank_cell_m) * Grid::gridspace), 2)
                                         + std::pow(((curr_cell_list->second - tank_cell_n) * Grid::gridspace), 2));

            std::cout << "Cell " << curr_cell_list->first << " " << curr_cell_list->second << std::endl;
            std::cout << "Distance " << cell_dist << std::endl;

            // function to calculate probability
            probability(curr_cell_list->first, curr_cell_list->second, cell_dist, Ultrasonic::Detection_distances[i]);

            // Remove the element from the list
            Grid::cell_list.erase(Grid::cell_list.begin());
        }
        std::cout << "Finished list" << std::endl;
    }
    return 0;
}

void probability(int cell_column, int cell_row, double cell_dist, double obj_dist) {

    // use a straight line function to start however i believe tanh would work nicely
<<<<<<< HEAD
    std::cout << "old prob"  << "="<< Grid::map[cell_column][cell_row] << std::endl;
    double prob = (0.2 * cell_dist) / obj_dist - 0.1;
    // set upper and lower prob lim
    // cap upper at 1
    if (Grid::map[cell_column][cell_row] + prob > 1) {
        Grid::map[cell_column][cell_row] = 1;
    }
    // cap lower at 0
    else if (Grid::map[cell_column][cell_row] < 0) {
        Grid::map[cell_column][cell_row] = 0;
    }
    else {
        Grid::map[cell_column][cell_row] += prob;
    }
    std::cout << "Probability " << prob << std::endl;
}

void breshams_alg(int i, double sen_hori[], double sen_vert[]) {
    double dx = sen_hori[i] - Localisation::w_Tank_Position[0];  // dx=x2-x1
    double dy = sen_vert[i] - Localisation::w_Tank_Position[1];  // dy=y2-y1
    double de = std::abs((dy / dx));                             // de=abs(dy/dx)
    double e  = 0;
    double y  = Localisation::w_Tank_Position[1];

    std::cout << "dx " << dx << std::endl;
    std::cout << "dy " << dy << std::endl;
    std::cout << "de " << de << std::endl;
    std::cout << "e  " << e << std::endl;
    std::cout << "y  " << y << std::endl;

    for (double x = Localisation::w_Tank_Position[0]; x <= sen_hori[i]; x += Grid::gridspace) {
        Grid::cell_list.emplace_back(std::make_pair(std::floor(x / Grid::gridspace), std::floor(y / Grid::gridspace)));
        e += de;

        std::cout << "Position " << x << " of " << sen_hori[i] << std::endl;
        std::cout << "Cell " << std::floor(x / Grid::gridspace) << " " << std::floor(y / Grid::gridspace) << std::endl;

        if (e >= 0.5) {
            y += (std::signbit(dy) == 1 ? 1 : -1) * Grid::gridspace;
            e -= Grid::gridspace;
            std::cout << "e overflow: y = " << y << " e = " << e << std::endl;
        }
    }
}

void Print_Occupancy_Map() {
    for (int rows = 0; rows < Grid::m; rows++) {
        for (int cols = 0; cols < Grid::n; cols++) {
            std::cout << Grid::map[rows][cols] << "\t";
        }
        std::cout << "\n" << std::endl;
    }
}

void Image_Occupancy_Map() {
    unsigned char* Map_Image = new unsigned char[1500 * 1500];
    for (int rows = 0; rows < Grid::m; rows++) {
        for (int cols = 0; cols < Grid::n; cols++) {
            for (int pixel = 0; pixel < 100; pixel++) {
                Map_Image[pixel * Grid::n + (cols * Grid::m + rows)] = Grid::map[rows][cols] * 255;
            }
        }
    }
    std::ofstream outFile("Map_Image.ppm", std::ios::binary);
    outFile << "P6\n" << 1500 << " " << 1500 << " 255\n";
    outFile.write((char*) Map_Image, 1500 * 1500);
    std::cout << "Image saved at Map_Image.ppm" << std::endl;
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
