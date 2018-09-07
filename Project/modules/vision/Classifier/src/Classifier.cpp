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