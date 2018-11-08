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

    // std::cout << "Start row " << (int) Grid::start_row << " Start column " << (int) Grid::start_column << std::endl;

    // what cell is the tank in, note that the x is forward and y is sideways, 0,0 is defined to be the bottom right
    //     corner -return a x and y for what cell we are looking at stuff in
    int tank_cell_m = std::floor(Localisation::w_Tank_Position[0] / Grid::gridspace) + Grid::start_row;
    int tank_cell_n = std::floor(Localisation::w_Tank_Position[1] / Grid::gridspace) + Grid::start_column;

    // std::cout << "Tank cell x " << tank_cell_m << std::endl;
    // std::cout << "Tank cell y " << tank_cell_n << std::endl;

    // these are some arrays to store "world" information based on sensors readings
    double sen_hori[4]  = {0};
    double sen_vert[4]  = {0};
    double sen_theta[4] = {0};

    // now the matrixes have been used instead, can use a loop to calculate these, probs a bit neater
    // what cell are we looking at stuff in- we need to convert the dist and angle to y,x coorinates

    // Front sonic (array pos=0)
    sen_theta[0] = Localisation::w_Tank_Rotation;

    // Right sonic (array pos=1)
    sen_theta[1] = Localisation::w_Tank_Rotation + M_PI + M_PI_2;

    // Back sonic (array pos=2)
    sen_theta[2] = Localisation::w_Tank_Rotation + M_PI;

    // Left sonic (array pos=3)
    sen_theta[3] = Localisation::w_Tank_Rotation + M_PI_2;

    // convert all of these to grid spaces and within here update occupancy map
    for (int i = 0; i < SENSORS; i++) {

        // std::cout << "Sensor " << i << std::endl;

        // account for the possibility of getting an invlid sensor reading, the distance will read -1
        if (Ultrasonic::Detection_distances[i] != -1) {
            sen_hori[i] = Ultrasonic::Detection_distances[i] * std::cos(sen_theta[i]);
            sen_vert[i] = Ultrasonic::Detection_distances[i] * std::sin(sen_theta[i]);
            // std::cout << "Sensor rotation " << sen_theta[i] << " Sensor hori " << sen_hori[i] << " Sensor vert "
            // << sen_vert[i] << std::endl;


            int object_cell_m = tank_cell_m + std::floor(sen_hori[i] / Grid::gridspace);
            int object_cell_n = tank_cell_n + std::floor(sen_vert[i] / Grid::gridspace);

            // std::cout << "Cell " << object_cell_m << " " << object_cell_n << std::endl;

            // startign closest to the tank, look at all the grid squares in the way by converting these points to a
            // straight line
            // y=mx+b for now we are using Bresham's algorithim
            // std::cout << "Calculating list of cells" << std::endl;

            // TODO Fix this
            // breshams_alg(i, sen_hori, sen_vert);
            if (object_cell_m <= Grid::m || object_cell_n <= Grid::n || object_cell_m >= 0 || object_cell_n >= 0) {
                Digital_Differential_Analyzer(object_cell_m, object_cell_n, tank_cell_m, tank_cell_n);

                // std::cout << "Fininished calculating list" << std::endl;

                while (!Grid::cell_list.empty()) {

                    std::vector<std::pair<int, int>>::const_iterator curr_cell_list = Grid::cell_list.begin();

                    // this will give a list of grid ms and ns that need to be looked at,starting from
                    // closest to the tank
                    // need to access the list pairs seperately and save as cella(m) and cellb(n)

                    // use pythagoras to calculate distance between tank and the cell we are looking at
                    double cell_dist =
                        std::sqrt(std::pow(((curr_cell_list->first - tank_cell_m) * Grid::gridspace), 2)
                                  + std::pow(((curr_cell_list->second - tank_cell_n) * Grid::gridspace), 2));

                    // std::cout << "Cell " << curr_cell_list->first << " " << curr_cell_list->second << std::endl;
                    // std::cout << "Distance " << cell_dist << std::endl;

                    if (cell_dist <= std::sqrt(std::pow(((Grid::m) *Grid::gridspace), 2)
                                               + std::pow(((Grid::n) *Grid::gridspace), 2))) {

                        // std::cout << "Cell " << curr_cell_list->first << " " << curr_cell_list->second << std::endl;
                        // std::cout << "Distance " << cell_dist << std::endl;

                        // function to calculate probability
                        probability(curr_cell_list->first,
                                    curr_cell_list->second,
                                    cell_dist,
                                    Ultrasonic::Detection_distances[i]);
                    }
		    else {
			std::cout << "ERROR Cell distance too large" << std::endl;
		    }
                    // Remove the element from the list
                    Grid::cell_list.erase(Grid::cell_list.begin());
                }
                // std::cout << "Finished list" << std::endl;
            }
        }
        else {
            // std::cout << "Sensor reading invalid" << std::endl;
        }
    }
    return 0;
}

void probability(int cell_column, int cell_row, double cell_dist, double obj_dist) {

    // use a straight line function to start however i believe tanh would work nicely
    // std::cout << "Previous probability " << Grid::map[cell_column][cell_row] << std::endl;

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
    // std::cout << "Probability " << prob << std::endl;
}

// TODO this doesn't use the vertical distance
void breshams_alg(int i, double sen_hori[], double sen_vert[]) {
    double dx = sen_hori[i] - Localisation::w_Tank_Position[0];  // dx=x2-x1
    double dy = sen_vert[i] - Localisation::w_Tank_Position[1];  // dy=y2-y1
    double de = std::abs((dy / dx));                             // de=abs(dy/dx)
    double e  = 0;
    double y  = Localisation::w_Tank_Position[1];

    for (double x = Localisation::w_Tank_Position[0]; x <= sen_hori[i]; x += Grid::gridspace) {
        Grid::cell_list.emplace_back(std::make_pair(std::floor(x / Grid::gridspace), std::floor(y / Grid::gridspace)));
        e += de;

        // std::cout << "Position " << x << " of " << sen_hori[i] << std::endl;
        // std::cout << "Cell " << std::floor(x / Grid::gridspace) << " " << std::floor(y / Grid::gridspace) <<
        // std::endl;

        if (e >= 0.5) {
            y += (std::signbit(dy) == 1 ? 1 : -1) * Grid::gridspace;
            e -= Grid::gridspace;
        }
    }
}


void Digital_Differential_Analyzer(double object_cell_m, double object_cell_n, double tank_cell_m, double tank_cell_n) {
    double x;
    double y;
    double x1 = tank_cell_n;
    double y1 = tank_cell_m;
    double x2 = object_cell_n;
    double y2 = object_cell_m;
    double dx;
    double dy;
    double step;
    int i;

    dx = (x2 - x1);
    dy = (y2 - y1);
    if (std::abs(dx) >= std::abs(dy)) {
        step = std::abs(dx);
    }
    else {
        step = std::abs(dy);
    }
    dx = dx / step;
    dy = dy / step;
    x  = x1;
    y  = y1;
    i  = 1;
    while (i <= step) {
        // std::cout << "Cell " << std::round(x) << "\t" << std::round(y) << std::endl;
        Grid::cell_list.emplace_back(std::make_pair(std::round(x), std::round(y)));
        x = x + dy;
        y = y + dx;
        i = i + 1;
        if (x > Grid::n || y > Grid::n) {
            break;
        }
        else if (x < 0 || y < 0) {
            break;
        }
    }
    if (x <= Grid::n || y <= Grid::n || x >= 0 || y >= 0) {
        // std::cout << "Cell " << std::round(x) << "\t" << std::round(y) << std::endl;
        Grid::cell_list.emplace_back(std::make_pair(std::round(x), std::round(y)));
    }
    else {
	std::cout << "ERROR Incorrect DDA cell" << std::endl;
    }
}

void Print_Occupancy_Map() {
    // Number of pixels each grid space is scaled to
    int pixel_cols_max       = 100;
    int pixel_row_max        = 100;
    unsigned char* Map_Image = new unsigned char[Grid::m * pixel_row_max * Grid::n * pixel_cols_max];
    for (int rows = 0; rows < Grid::m; rows++) {
        for (int pixel_rows = 0; pixel_rows < pixel_row_max; pixel_rows++) {
            for (int cols = 0; cols < Grid::n; cols++) {
                for (int pixel_cols = 0; pixel_cols < pixel_cols_max; pixel_cols++) {
                    Map_Image[Grid::m * pixel_row_max * pixel_cols_max * rows
                              + (Grid::n * pixel_row_max * pixel_rows + (pixel_cols + cols * pixel_cols_max))] =
                        (1 - Grid::map[rows][cols]) * 255;
                }
            }
        }
    }
    std::ofstream outFile("Map_Image.pgm", std::ios::binary);
    outFile << "P5\n" << Grid::m * pixel_row_max << " " << Grid::n * pixel_cols_max << " 255\n";
    outFile.write((char*) Map_Image, Grid::m * pixel_row_max * Grid::n * pixel_cols_max);
    // std::cout << "Image saved at Map_Image. " << std::endl;
    delete Map_Image;
}
