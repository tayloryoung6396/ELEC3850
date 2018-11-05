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

#include "stdafx.h"
#define WIDTH_MIN 100
#define WIDTH_MAX 200
#define HEIGHT_MIN 100
#define HEIGHT_MAX 200


#define IMAGE_HEIGHT 1280
#define IMAGE_WIDTH 960

int Image_Size = IMAGE_HEIGHT * IMAGE_WIDTH * 3;
//colours[How many colours][R,G,B][Min,Max]
int colours[3][3][2] = { { { 64,97 },{ 19,48 },{ 19,47 } },		//Red maximum and minimum pixel parameters (RGB Image)
{ { 1,9 },{ 11,49 },{ 47,61 } },		//Green maximum and minimum pixel parameters (RGB Image)
{ { 14,33 },{ 44,61 },{ 79,111 } },		//Blue maximum and minimum pixel parameters (RGB Image)
};

//int seed[][] = { {colour, pixel}, {} }
int seed[][] = ;

//int object[][] = {{center_x, center_y, width, height}, {} }
int object[][] = ;

void Classifier(uint8_t* data)
{
	int pixel = 0;		//start at the first pixel of the image 
	int colour = 0;
	int i = 0;
	int k = 0;

	for (pixel = 0; pixel <= Image_Size; pixel += 3) {

		for (colour = 0; colour <= 3; colour++) {

			if (pixel >= colours[colour][0][0] && pixel <= colours[colour][0][1]
				&& (pixel + 1) >= colours[colour][1][0] && (pixel + 1) <= colours[colour][1][1]
				&& (pixel + 2) >= colours[colour][2][0] && (pixel + 1) <= colours[colour][2][1]
				)
			{
				seed[i][0] = colour;
				seed[i][1] = pixel;
				i++;

				break;
			}

		}

	}

	//Insert some for loop to cancel out seed points included in previously found objects 

	//cycle through seed points
	for (j = 0; j <= i; j++) {

		int left = 0;
		int right = 0;
		int up = 0;
		int down = 0;

		colour = seed[j][0];
		pixel = seed[j][1];

		R_min = colours[colour][0][0];
		R_max = colours[colour][0][1];
		G_min = colours[colour][1][0];
		G_max = colours[colour][1][1];
		B_min = colours[colour][2][0];
		B_max = colours[colour][2][1];


		//cluster right
		while (R_min <= data[pixel] && R_max >= data[pixel]
			&& G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)]
			&& B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]
			) {

			right++;
			pixel += 3;
		}

		//cluster left
		pixel = seed[j][1];
		while (R_min <= data[(pixel - 3)] && R_max >= data[(pixel - 3)]
			&& G_min <= data[(pixel - 2)] && G_max >= data[(pixel - 2)]
			&& B_min <= data[(pixel - 1)] && B_max >= data[(pixel - 1)]
			) {

			left++;
			pixel = pixel - 3;
		}

		//cluster up
		pixel = seed[j][1];
		while (R_min <= data[pixel] && R_max >= data[pixel]
			&& G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)]
			&& B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]
			) {

			up++;
			pixel = pixel - IMAGE_WIDTH / 3;
		}

		//cluster down
		pixel = seed[j][1];
		while (R_min <= data[pixel] && R_max >= data[pixel]
			&& G_min <= data[(pixel + 1)] && G_max >= data[(pixel + 1)]
			&& B_min <= data[(pixel + 2)] && B_max >= data[(pixel + 2)]
			) {

			down++;
			pixel = pixel + IMAGE_WIDTH / 3;
		}

		//calulate object width and height from clustering
		int width = left + right;
		int height = up + down;

		//Check object size to target object parameters

		if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

			//Calculate centers
			pixel = seed[j][1];
			int center_x = ((pixel - left) + (pixel + right)) / 2;
			int center_y = ((pixel - up * (IMAGE_WIDTH / 3)) + (pixel + down * (IMAGE_WIDTH / 3))) / 2;

			object[k][0] = center_x;
			object[k][1] = center_y;
			object[k][2] = width;
			object[k][3] = height;

			k++;
		}
	}

	return;
}

