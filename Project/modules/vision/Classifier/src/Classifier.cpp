/*
 * This should handle object detection of the images.
 * Figue out vector to object of interest from image.
 * Inputs:  3 x Greyscale images
 * Outputs: Vector to objects of interest
 * Red: (255, 0, 0)
 * Green: (0, 255, 0)
 * Blue: (0, 0, 255)
 * Yellow: (255, 255, 0)
 * Brown: (165, 42, 42)
 * Orange: (255, 165, 0)
 * Black: (0, 0, 0)
 * White: (255, 255, 255)
 */

#include "Classifier.hpp"

void Classifier_init() {
    std::cout << "Initilising CLASSIFIER" << std::endl;
}

int Classifier_main() {
    // // Two methods that imediately come to mind


    // // Method 1: Greyscale colour detection
    // // Take the image matrix
    // // Split into N grayscale images centered around a set of specific colours
    // // This would essentially take and rgb colour (a, b, c)
    // // And scale the colour of each pixel against this reference
    // // Another extra option could be down scaling the image in this step
    // // Process()

    // // Method 2: RGB colour detection
    // // Do the object detection over the whole spectrum
    // // Process()


    // // Process()
    // // Take an image (rgb or grayscale)
    // // Perform a search pattern over every K pixels (no need to search them all) (many ways to do this)
    // // Make a list of seed points that are within the specific colour range
    // // Once the image has been searched perform clustering

    // Seed() {
    //     for (int rows = 0; rows < image.size(0); rows += search_factor) {
    //         for (int cols = 0; cols < image.size(1); cols += search_factor) {
    //             // Check to see if this pixel matches one of our colour types
    //             for (int colour = 0; colour < num_colours; colour++) {
    //                 image.pixel(rows, cols);
    //                 // if it does store the x, y position in a list of seed points
    //                 // Probably some array of pairs
    //             }
    //         }
    //     }
    // }

    // // Clustering()
    // // Clustering involves starting at a seed and working your way left right up and down
    // // until you find a transition point from objective colour to non objective colour
    // // then you find the area of that space
    // // and determine the center and therefore roughly how far away it is
    // // from that there is a calculation to determine if the object is within the size expected
    // // based on how far away it is
    // // This if approved gets added to a list
    // Cluster() {
    //     // For each colour type
    //     for (int colour = 0; colour < num_colours; colour++) {
    //         // Cluster any seed points into a list of width height center parameters
    //         // NOTE: a seed point that is within a cluster boundary is not then clustered
    //         for (int seed = 0; seed < ; seed++) {
    //             // If the seed doesnt fall in a previous cluster, check it
    //             if (seed not in cluster) {
    //                 // Check from the seed what the min and max height and width of the cluster is
    //                 // There's probably a better way to do this next step, just not entirely sure how / don't use
    //                 this..
    //                 // it's bad Search left right up down
    //                 int cluster_x = seed_list[seed, 0];
    //                 int cluster_y = seed_list[seed, 1];
    //                 while(image.pixel(cluster_x, cluster_y) == we are happy with the colour?){
    //                     cluster_x++;
    //                 }
    //                 cluster_x_max = cluster_x - 1;
    //                 // Search left right up down
    //                 cluster_x = seed_list[seed, 0];
    //                 cluster_y = seed_list[seed, 1];
    //                 while(image.pixel(cluster_x, cluster_y) == we are happy with the colour?){
    //                     cluster_x--;
    //                 }
    //                 cluster_x_min = cluster_x + 1;
    //                 // Search left right up down
    //                 cluster_x = seed_list[seed, 0];
    //                 cluster_y = seed_list[seed, 1];
    //                 while(image.pixel(cluster_x, cluster_y) == we are happy with the colour?){
    //                     cluster_y++;
    //                 }
    //                 cluster_y_max = cluster_y - 1;
    //                 // Search left right up down
    //                 cluster_x = seed_list[seed, 0];
    //                 cluster_y = seed_list[seed, 1];
    //                 while(image.pixel(cluster_x, cluster_y) == we are happy with the colour?){
    //                     cluster_y--;
    //                 }
    //                 cluster_y_min = cluster_y + 1;

    //                 // Now that we have the limits of our cluster, find half the height and width and the center
    //                 point center_x = (cluster_x_max - cluster_x_min) / 2 + cluster_x_min; center_y = (cluster_y_max -
    //                 cluster_y_min) / 2 + cluster_y_min; width    = cluster_x_max - cluster_x_min; height   =
    //                 cluster_y_max - cluster_y_min;
    //                 // Store this in some list of clusters
    //             }
    //         }
    //     }
    // }

    // // Now we have some sort of list of clusters for each colour
    // // Each of these lists contain a list of clusters, which are a tuple of center, width and height
    // // Now do something with these..
    // // Check that a cluster is valid
    // // eg is it within some size that we expect
    // // Probably do some distance estimation and what not
    // // Something
    return 0;
}

// ClassificationT2.cpp : Defines the entry point for the console application.
//

#define WIDTH_MIN 100
#define WIDTH_MAX 300
#define HEIGHT_MIN 100
#define HEIGHT_MAX 200


#define IMAGE_HEIGHT 960
#define IMAGE_WIDTH 1280

int Image_Size = IMAGE_HEIGHT * IMAGE_WIDTH * 3;

int colours[3][3][2] = { { { 64 , 97 },{ 19,48 },{ 19,47 } }, //rgb red
{ { 75,190 },{ 41,225 },{ 14,54 } },						  //hsv green
{ { 200,218 },{ 104,239 },{ 58,146 } } };					  //hsv blue

															  //int seed[][] = { {colour, pixel}, {} }
int seed[100][2] = { { 0,0 },{ 0,0 } };

//int object[][] = {{center_x, center_y, width, height}, {} }
int object[40][4] = { { 0,0,0,0 },{ 0,0,0,0 } };

void Classifier(uint8_t* data)
{
	//printf("into classifier \n");

	uint8_t* data_mod = new uint8_t[Image_Size];
	data_mod = data;

	int pixel = 0;		//start at the first pixel of the image 
	int colour = 0;
	int i = 0;
	int k = 0;
	int objects_found = 0;

	rgb rgb_colour;
	hsv hsv_colour;
	int h_value = 0;
	int s_value = 0;
	int v_value = 0;

	for (pixel = 0; pixel <= Image_Size; pixel += 4200) {
		//printf("into pixel for loop \n");
		for (colour = 0; colour < 3; colour++) {
			//printf("into colour for loop \n");

			if (colour == 0
				&& data[pixel] >= colours[colour][0][0] && data[pixel] <= colours[colour][0][1]
				&& data[(pixel + 1)] >= colours[colour][1][0] && data[(pixel + 1)] <= colours[colour][1][1]
				&& data[(pixel + 2)] >= colours[colour][2][0] && data[(pixel + 2)] <= colours[colour][2][1])
			{
				seed[i][0] = colour;
				seed[i][1] = pixel;
				i++;
				break;

			}
			else if (colour == 1 || colour == 2)
			{
				rgb_colour.r = data[pixel] / 255.0;
				rgb_colour.g = data[pixel + 1] / 255.0;
				rgb_colour.b = data[pixel + 2] / 255.0;
				hsv_colour = rgb2hsv(rgb_colour);
				h_value = hsv_colour.h;
				s_value = hsv_colour.s * 255;
				v_value = hsv_colour.v * 255;

				if (h_value >= colours[colour][0][0] && h_value <= colours[colour][0][1]
					&& s_value >= colours[colour][1][0] && s_value <= colours[colour][1][1]
					&& v_value >= colours[colour][2][0] && v_value <= colours[colour][2][1]
					)
				{
					seed[i][0] = colour;
					seed[i][1] = pixel;
					i++;
					break;
				}
			}
		}

	}
	printf("seeds = %d\n", i);

	//cycle through seed points
	for (int j = 0; j <= (i - 5); j++) {
		//printf("into seed point \n");
		colour = seed[j][0];
		pixel = seed[j][1];
		int m = 0;
		int seed_y = (seed[j][1] / 3.0) / IMAGE_WIDTH;
		int seed_x = (seed[j][1] / 3.0) - seed_y*IMAGE_WIDTH;
		bool seed_in_object = 0;

		if (objects_found > 0) {
			for (m = 0; m <= objects_found; m++) {
				//printf("into seed object check \n");
				int errorx = 200;
				int errory = 50;

				int object_center = IMAGE_WIDTH * 3 * object[m][1] + object[m][0] * 3;
				int ax = object[m][1] - object[m][3] * 3 / 2.0;
				int ay = object[m][0] - object[m][2] * 3 / 2.0;
				int bx = object[m][1] + object[m][3] * 3 / 2.0;
				int by = object[m][0] + object[m][2] * 3 / 2.0;

				//printf("Object Center = %d \t ax = %d \t ay = %d \t bx = %d \t by = %d \t\n", object_center, ax, ay, bx, by);
				//printf("Seed = %d \t Seed_x = %d \t Seed_y = %d \t\n\n", pixel, seed_x, seed_y);

				if (seed_x >= (ax - errorx) && seed_x <= (bx + errorx) && seed_y >= ay - (errory) && seed_y <= (by + errory)) {
					//seed is already in an object 
					seed_in_object = 1;
					//printf("SEED IN OBJECT \n\n");
					break;
				}

			}
		}

		if (seed_in_object == 1) {
			//printf("SEED IN OBJECT IF LOOP BREAK \n");

		}

		else {

			//printf("into else, seed not in object \n");
			int left = 0;
			int right = 0;
			int up = 0;
			int down = 0;

			int R_min = colours[colour][0][0];
			int R_max = colours[colour][0][1];
			int G_min = colours[colour][1][0];
			int G_max = colours[colour][1][1];
			int B_min = colours[colour][2][0];
			int B_max = colours[colour][2][1];


			//cluster right

			int error = 0;

			while (error < 11) {

				int a = 0;
				if (seed[j][0] == 0) {

					h_value = data[pixel];
					s_value = data[pixel + 1];
					v_value = data[pixel + 2];

				}

				if (seed[j][0] == 1 || seed[j][0] == 2) {

					rgb_colour.r = data[pixel] / 255.0;
					rgb_colour.g = data[pixel + 1] / 255.0;
					rgb_colour.b = data[pixel + 2] / 255.0;
					hsv_colour = rgb2hsv(rgb_colour);
					h_value = hsv_colour.h;
					s_value = hsv_colour.s * 255;
					v_value = hsv_colour.v * 255;
				}

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {
					a = 1;

				}
				if (a != 1) {
					error++;
				}
				right++;

				//data_mod[pixel + 0] = { 55 };
				//data_mod[pixel + 1] = { 155 };
				//data_mod[pixel + 2] = { 125 };

				pixel += 3;
			}


			//cluster left
			error = 0;
			pixel = seed[j][1];

			while (error < 11) {

				int a = 0;
				if (seed[j][0] == 0) {

					h_value = data[pixel - 3];
					s_value = data[pixel - 2];
					v_value = data[pixel - 1];

				}

				if (seed[j][0] == 1 || seed[j][0] == 2) {
					rgb_colour.r = data[pixel - 3] / 255.0;
					rgb_colour.g = data[pixel - 2] / 255.0;
					rgb_colour.b = data[pixel - 1] / 255.0;
					hsv_colour = rgb2hsv(rgb_colour);
					h_value = hsv_colour.h;
					s_value = hsv_colour.s * 255;
					v_value = hsv_colour.v * 255;
				}

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {

					a = 1;
				}

				if (a != 1) {
					error++;
				}

				left++;

				//data_mod[pixel - 3] = { 255 };
				//data_mod[pixel - 2] = { 55 };
				//data_mod[pixel - 1] = { 125 };

				pixel = pixel - 3;
			}

			//cluster up
			error = 0;
			pixel = seed[j][1];

			while (error < 11) {

				int a = 0;
				if (seed[j][0] == 0) {

					h_value = data[pixel];
					s_value = data[pixel + 1];
					v_value = data[pixel + 2];

				}

				if (seed[j][0] == 1 || seed[j][0] == 2) {

					rgb_colour.r = data[pixel] / 255.0;
					rgb_colour.g = data[pixel + 1] / 255.0;
					rgb_colour.b = data[pixel + 2] / 255.0;
					hsv_colour = rgb2hsv(rgb_colour);
					h_value = hsv_colour.h;
					s_value = hsv_colour.s * 255;
					v_value = hsv_colour.v * 255;
				}

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {

					int p = (pixel / 3.0) / IMAGE_WIDTH;
					int q = (pixel / 3.0) - p *IMAGE_WIDTH;

					a = 1;
				}

				if (a != 1) {
					error++;
				}
				up++;

				//data_mod[pixel + 0] = { 55 };
				//data_mod[pixel + 1] = { 255 };
				//data_mod[pixel + 2] = { 25 };

				pixel = pixel - IMAGE_WIDTH * 3;
			}

			//cluster down
			error = 0;
			pixel = seed[j][1];
			while (error < 11) {

				int a = 0;
				if (seed[j][0] == 0) {

					h_value = data[pixel];
					s_value = data[pixel + 1];
					v_value = data[pixel + 2];

				}

				if (seed[j][0] == 1 || seed[j][0] == 2) {
					rgb_colour.r = data[pixel] / 255.0;
					rgb_colour.g = data[pixel + 1] / 255.0;
					rgb_colour.b = data[pixel + 2] / 255.0;
					hsv_colour = rgb2hsv(rgb_colour);
					h_value = hsv_colour.h;
					s_value = hsv_colour.s * 255;
					v_value = hsv_colour.v * 255;
				}

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {

					a = 1;
				}

				if (a != 1) {
					error++;
				}
				down++;

				pixel = pixel + IMAGE_WIDTH * 3;
			}

			//calulate object width and height from clustering
			int width = left + right;
			int height = up + down;

			if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {


				objects_found++;

				//Calculate centers
				pixel = seed[j][1];
				int y = (seed[j][1] / 3.0) / IMAGE_WIDTH;
				int x = (seed[j][1] / 3.0) - y*IMAGE_WIDTH;

				int center_x = ((x - left) + (x + right)) / 2.0;
				int center_y = ((y - up) + (y + down)) / 2.0;

				object[k][0] = center_x;
				object[k][1] = center_y;
				object[k][2] = width;
				object[k][3] = height;

				//printf("Object %d, width %d, height %d, center x %d, center y %d, width %d height %d\n", objects_found, width, height, center_x, center_y, width, height);
				//printf("pixel = %d, left = %d, right = %d, y = %d, x = %d, up = %d, down = %d\n",  pixel, left, right, y, x, up, down);

				int obj_center[2] = { center_x, center_y };
				Output_Segmentation(data_mod, IMAGE_WIDTH, IMAGE_HEIGHT, width, height, obj_center);

				//Find_distance(center_x, center_y);

				printf("Object found seed = %d\n", object[k][1]);

				k++;


			}
		}
	}
	printf("Objects found = %d \n", objects_found);



	printf("Saving file\n");
	std::ofstream outfile("Segmented_Image_3.ppm", std::ios::binary);
	outfile << "P6\n" << IMAGE_WIDTH << " " << IMAGE_HEIGHT << " 255\n";  // dont know
	outfile.write((char*)data_mod, (Image_Size));
	printf("Segmented_Image Saved\n");

	delete data_mod;
	return;
}
