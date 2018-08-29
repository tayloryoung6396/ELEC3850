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
    // Two methods that imediately come to mind


    // Method 1: Greyscale colour detection
    // Take the image matrix
    // Split into N grayscale images centered around a set of specific colours
    // This would essentially take and rgb colour (a, b, c)
    // And scale the colour of each pixel against this reference
    // Another extra option could be down scaling the image in this step
    // Process()

    // Method 2: RGB colour detection
    // Do the object detection over the whole spectrum
    // Process()


    // Process()
    // Take an image (rgb or grayscale)
    // Perform a search pattern over every K pixels (no need to search them all) (many ways to do this)
    // Make a list of seed points that are within the specific colour range
    // Once the image has been searched perform clustering

    // Clustering()
    // Clustering involves starting at a seed and working your way left right up and down
    // until you find a transition point from objective colour to non objective colour
    // then you find the area of that space
    // and determine the center and therefore roughly how far away it is
    // from that there is a calculation to determine if the object is within the size expected
    // based on how far away it is
    // This if approved gets added to a list
    return 0;
}