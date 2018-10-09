/*
 * This should take the Ultrasonic readings and the current position and populate an occupancy map of the surroundings
 * Inputs:  Ultrasonic sensor readings
 *          Position
 * Outputs: Occupancy map
 *
 */


#include "Localisation.hpp"

double Localistation::w_Tank_Position[2] = {0};  // X Y Current position in world coordinates
double Localistation::w_Tank_Rotation    =  0 ;       // Rotation about the w_Tank_Position in the anticlockwise direction (radians)
double Localistation::w_Goal_Position[2] = {0};  // X Y Goal position in world coordinates

double sen_hori[4];
double sen_vert[4];
double sen_theta[4];

std::vector<std::pair<double, double>>::const_iterator curr_cell_list;
static std::vector<std::pair<double, double>> cell_list;

void Localisation_init() {
    std::cout << "Initilising LOCALISATION" << std::endl;
}

int Localisation_main() {

    int m=15,                                      //dimensions that are mxn
            n=m,
            start_row=m,                           //assume starting position is in the middle bottom (for robot)
            start_column=m/2;                      //start pos is more for path planner, dont really need here
    double area_size=1.5,                          //this is the dimensions of the grid in Y*Y meters
            gridspace=0.1,                         //this is the dimensions of the gridsquares in Y*Y meters
            grid_map[m][n]={0.5};                  //m x n matrix that serves as the occupancy map

    int tank_cell_m=floor(w_Tank_Position[0]/gridspace),
            tank_cell_n=floor(w_Tank_Position[1]/gridspace);

    //these are some arrays to store "world" information based on sensors readings
    double sen_hori[4],
            sen_vert[4],
            sen_theta[4];

    for(int i=0; i<4; i++){
                //where array position 0 is front, array position 1 is
        sen_theta[i]=w_Tank_Rotation+ M_PI_2*i;
        sen_hori[i]=Detection_distances[i]*cos(sen_theta[i]);
        sen_vert[i]=Detection_distances[i]*sin(sen_theta[i]);
        }




    return 0;
}


/*
// convert tank to world
void TankToWorld(double w_Tank_Position[2]) {
    //what cell is the tank in, note that the x is forward and y is sideways, 0,0 is defined to be the bottom right
//     corner -return a x and y for what cell we are looking at stuff in
    int tank_cell_m=floor(w_Tank_Position[0]/gridspace);
    int tank_cell_n=floor(w_Tank_Position[1]/gridspace);
}


float ObjectToWorld( ){

}
*/
// // 2D Map
// // This is a crude method of creating a 2D localisation map
// // Given some fixed workspace, divide it into a square grid section
// // For this implementation 1.5m x 1.5m with a grid spacing of 0.100- based off a tank diagonal size of 440mm
    double area_size=1.5;

// //note this will mean that you need 4 vacant grid spaces to pass through
// //this may need to be changed based on run time
    double gridspace=0.1;

// // grid_map[15][15]
// //for a grid tht is x[m][n]
    int m=15;
    int n=m;
// //this is a square :)
// // create m by n matrix- note this needs to be a double as we are looking at probabilities- could change if this is
// too slow
    double grid_map[m][n]={0.5};


// //assume starting position is in the middle bottom
    int start_row=m;
    int start_column=m/2;


    int (w_Tank_Position, w_Tank_Rotation, Detection_distances, area_size)
    {
    //what cell is the tank in, note that the x is forward and y is sideways, 0,0 is defined to be the bottom right
//     corner -return a x and y for what cell we are looking at stuff in
    int tank_cell_m=floor(w_Tank_Position[0]/gridspace);
    int tank_cell_n=floor(w_Tank_Position[1]/gridspace);

//these are some arrays to store "world" information based on sensors readings
    double sen_hori[4];
    double sen_vert[4];
    double sen_theta[4];

    for(int i=0; i<4; i++){
                //where array position 0 is front, array position 1 is
        sen_theta[i]=w_Tank_Rotation+ M_PI_2*i;
        sen_hori[i]=Detection_distances[i]*cos(sen_theta[i]);
        sen_vert[i]=Detection_distances[i]*sin(sen_theta[i]);
        }
    for (int i=0; i<4; i++){
         int object_cell_m= tank_cell_m+ floor(sen_hori[i]/gridspace);
         int object_cell_n= tank_cell_n+ floor(sen_vert[i]/gridspace);

         }


//     //startign closest to the tank, look at all the grid squares in the way by converting these points to a straight
//     line
//     // y=mx+b for now we are using Bresham's algorithim
        std::vector<std::pair<double, double>> cell_list = breshams_alg(i, sen_hori, sen_vert, w_Tank_Position)
            while (!cell_list.empty()) {   //this will give a list of grid ms and ns that need to be looked at,starting from closest to the tank
               //need to access the list pairs seperately and save as cella(m) and cellb(n)

            double cell_dist = sqrt(((cella-tank_cell_m)*gridspace)^2+((cellb-tank_cell_n)*gridspace)^2);// use pythagoras to calculate distance between tank and the cell we are looking at

                 probability(cell_list->first, cell_list->second, cell_dist, Detection_distances[i]);     //function to
//                 calculate probability

//                 //FUNCTION TO UPDATE GRID OCCUPANCY PROBABILITY

                 cell_list.erase(cell_list.begin());
//                                         }


 }
     }

 std::vector<std::pair<double, double>> breshams_alg(i, sen_hori, sen_vert, w_Tank_Position, gridspace)
 {
     double dx=sen_hori[i]-w_Tank_Position[0];   //dx=x2-x1
     double dy=sen_vert[i]-w_Tank_Position[1];   //dy=y2-y1
     double de=abs((dy/dx));                     //de=abs(dy/dx)
     double e=0;
     double y = w_Tank_Position[1];
     std::vector<std::pair<double, double>> cell_list;

     for (double x=w_Tank_Position[0], x<=sen_hori[i], gridspace){
        cell_list.emplace_back(std::make_pair(floor(x/gridspace), floor(y/gridspace));
         e+=de;
        if(e>=0.5){
           y+=(signbit(dy) == 1 : 1 ? -1) * gridspace;
            e-= gridspace;
        }

     }
     return cell_list;
 }

void probability(m, n, cell_dist, obj_dist){

    //use a straight line function to start however i believe tanh would work nicely
    double prob= (0.2*cell_dist)/obj_dist -0.1;
    grid_map[m][n]+=prob;
 }

// // grid_map[x][y] = transform_us_world(us_id, get_dist(us_id));

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

// ** now the matrixes have been used instead, can use a loop to calculate these, probs a bit neater
//>>>>>

//     //what cell are we looking at stuff in- we need to convert the dist and angle to y,x coorinates
//     //front sonic (array pos=0)looks the same direction as the tank
/*
    sen_theta[0]=w_Tank_Rotation;
    sen_hori[0]=Detection_distances[0]*cos(sen_theta[0]);
    sen_vert[0]=Detection_distances[0]*sin(sen_theta[0]);

//     //LHS wrt tank sonic (array pos=1)
    sen_theta[1]=w_Tank_Rotation + M_PI_2;
    sen_hori[1]=Detection_distances[1]*cos(sen_theta[1]);
    sen_vert[1]=Detection_distances[1]*sin(sen_theta[1]);

//     //back wrt tank sonic (array pos=2)
    sen_theta[2]=w_Tank_Rotation +M_PI ;
    sen_hori[2]=Detection_distances[2]*cos(sen_theta[2]);
    sen_vert[2]=Detection_distances[2]*sin(sen_theta[2]);

//     //rhs sonic (array pos=3)
    sen_theta[3]=w_Tank_Rotation+M_PI+M_PI_2;
    sen_hori[3]=Detection_distances[3]*cos(sen_theta[3]);
    sen_vert[3]=Detection_distances[3]*sin(sen_theta[3]);
      double sen_hori[4];
        double sen_vert[4];
        double sen_theta[4];
        for(int i=0; i<4; i++){
                //where array position 0 is front, array position 1 is
        sen_theta[i]=w_Tank_Rotation+ M_PI_2*i;
        sen_hori[i]=Detection_distances[i]*cos(sen_theta[i]);
        sen_vert[i]=Detection_distances[i]*sin(sen_theta[i]);
        }


//     // convert all of these to grid spaces and within here update occupancy map
//     //note, SENSORS is currently 3, but am assuming it will be changed to 4

     for (int i=0; i<4; i++){
         int object_cell_m= tank_cell_m+ floor(sen_hori[i]/gridspace);
         int object_cell_n= tank_cell_n+ floor(sen_vert[i]/gridspace);
*/
