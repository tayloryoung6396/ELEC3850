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

// http://www.cse.psu.edu/~rtc12/CSE486/lecture12.pdf
// http://www.cse.psu.edu/~rtc12/CSE486/lecture13.pdf

#include "Classifier.hpp"

#define OFFSET 5

int Classifier::colours[3][3][2] = {0};
int Classifier::seed[1000][2];
int Classifier::object[10][4];


void Classifier_init() {
    std::cout << "Initilising CLASSIFIER" << std::endl;
    const int colours[3][3][2] = {
        {{64, 255}, {0, 48}, {0, 47}},   // Red maximum and minimum pixel parameters (RGB Image)
        {{0, 9}, {11, 255}, {0, 44}},    // Green maximum and minimum pixel parameters (RGB Image)
        {{0, 33}, {0, 61}, {79, 255}}};  // Blue maximum and minimum pixel parameters (RGB Image)
    for (int q = 0; q < 3; q++) {
        for (int w = 0; w < 3; w++) {
            for (int e = 0; e < 2; e++) {
                Classifier::colours[q][w][e] = colours[q][w][e];
            }
        }
    }
}

void Classifier(uint8_t* data) {
    printf("into classifier \n");

    uint8_t* data_mod = new uint8_t[3 * Image::Width * Image::Height];
    data_mod          = data;

    int pixel         = 0;  // start at the first pixel of the image
    int colour        = 0;
    int i             = 0;
    int k             = 0;
    int objects_found = 0;
    int error         = 0;

    for (pixel = 0; pixel <= Image::Height * Image::Width * 3; pixel += 1800) {
        // printf("into pixel for loop \n");
        for (colour = 0; colour < 3; colour++) {
            // printf("into colour for loop \n");
            if (data[pixel] >= Classifier::colours[colour][0][0] && data[pixel] <= Classifier::colours[colour][0][1]
                && data[(pixel + 1)] >= Classifier::colours[colour][1][0]
                && data[(pixel + 1)] <= Classifier::colours[colour][1][1]
                && data[(pixel + 2)] >= Classifier::colours[colour][2][0]
                && data[(pixel + 2)] <= Classifier::colours[colour][2][1]) {
                Classifier::seed[i][0] = colour;
                Classifier::seed[i][1] = pixel;
                i++;
                break;
            }
        }
    }
    printf("seeds = %d\n", i);

    // cycle through seed points
    for (int j = 0; j <= i; j++) {
        // printf("into seed point \n");
        colour             = Classifier::seed[j][0];
        pixel              = Classifier::seed[j][1];
        int m              = 0;
        int seed_y         = (Classifier::seed[j][1] / 3.0) / Image::Width;
        int seed_x         = (Classifier::seed[j][1] / 3.0) - seed_y * Image::Width;
        int seed_in_object = 0;

        for (m = 0; m <= objects_found; m++) {
            // printf("into seed object check \n");
            int object_center = Image::Width * 3 * Classifier::object[m][1] + Classifier::object[m][0] * 3;
            int ax            = Classifier::object[m][1] - Classifier::object[m][3] * 3 / 2;
            int ay            = Classifier::object[m][0] - Classifier::object[m][2] * 3 / 2;
            int bx            = Classifier::object[m][1] + Classifier::object[m][3] * 3 / 2;
            int by            = Classifier::object[m][0] + Classifier::object[m][2] * 3 / 2;

            if (seed_x >= ax && seed_x <= bx && seed_y >= ay && seed_y <= by) {
                // seed is already in an object
                seed_in_object = 1;
                break;
            }
        }

        if (seed_in_object == 1) {
            // printf("SEED IN OBJECT IF LOOP BREAK \n");
        }

        else {
            // printf("into else, seed not in object \n");
            int left  = 0;
            int right = 0;
            int up    = 0;
            int down  = 0;

            int R_min = Classifier::colours[colour][0][0];
            int R_max = Classifier::colours[colour][0][1];
            int G_min = Classifier::colours[colour][1][0];
            int G_max = Classifier::colours[colour][1][1];
            int B_min = Classifier::colours[colour][2][0];
            int B_max = Classifier::colours[colour][2][1];

            // cluster right
            error = 0;
            while (error < 10) {
                if (R_min - OFFSET <= data[pixel] && R_max + OFFSET >= data[pixel]
                    && G_min - OFFSET <= data[(pixel + 1)] && G_max + OFFSET >= data[(pixel + 1)]
                    && B_min - OFFSET <= data[(pixel + 2)] && B_max + OFFSET >= data[(pixel + 2)]) {
                }
                else {
                    error++;
                }
                right++;

                data_mod[pixel]     = {255};
                data_mod[pixel + 1] = {255};
                data_mod[pixel + 2] = {0};

                pixel += 3;
            }

#define IMAGE_HEIGHT 960
#define IMAGE_WIDTH 1280

int Image_Size = IMAGE_HEIGHT * IMAGE_WIDTH * 3;
//colours[How many colours][R,G,B][Min,Max]
//int colours[3][3][2] = { { { 64,97 },{ 19,48 },{ 19,47 } },		//Red maximum and minimum pixel parameters (RGB Image)
//{ { 1,9 },{ 11,49 },{ 47,61 } },		//Green maximum and minimum pixel parameters (RGB Image)
//{ { 14,33 },{ 44,61 },{ 79,111 } },		//Blue maximum and minimum pixel parameters (RGB Image)
//};

int colours[3][3][2] = { { { 0 , 100 },{ 136,219 },{ 51,100 } },
{ { 75,190 },{ 41,225 },{ 14,54 } },
{ { 200,218 },{ 104,239 },{ 58,146 } } };

//int colours[3][3][2] = { { {51, 100}, { 0,40 }, { 0,41 }},
//{ { 2, 33 }, { 0,54 }, { 0,40 } },
//{ { 14,33 },{ 44,61 },{ 79,111 }} };//{ 5, 65 }, { 0,102 }, { 0,146 } }};

//int seed[][] = { {colour, pixel}, {} }
int seed[100][2] = { { 0,0 },{ 0,0 } };

            // cluster up
            pixel = Classifier::seed[j][1];
            error = 0;
            while (error < 10) {
                if (R_min - OFFSET <= data[pixel] && R_max + OFFSET >= data[pixel]
                    && G_min - OFFSET <= data[(pixel + 1)] && G_max + OFFSET >= data[(pixel + 1)]
                    && B_min - OFFSET <= data[(pixel + 2)] && B_max + OFFSET >= data[(pixel + 2)]) {
                }
                else {
                    error++;
                }
                up++;
                data_mod[pixel]       = {25};
                data_mod[(pixel + 1)] = {255};
                data_mod[(pixel + 2)] = {200};

                pixel = pixel - Image::Width * 3;
            }

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

                pixel = pixel + Image::Width * 3;
            }

	}
	//printf("seeds = %d\n", i);

	//cycle through seed points
	for (int j = 0; j <= i; j++) {
		//printf("into seed point \n");
		colour = seed[j][0];
		pixel = seed[j][1];
		int m = 0;
		int seed_y = (seed[j][1] / 3.0) / IMAGE_WIDTH;
		int seed_x = (seed[j][1] / 3.0) - seed_y*IMAGE_WIDTH;
		bool seed_in_object = 0;

                // printf("Pixel new %d, %d\n", q, p);

		for (m = 0; m <= objects_found; m++) {
			//printf("into seed object check \n");
			int errorx = 200;
			int errory = 50;

			int object_center = IMAGE_WIDTH * 3 * object[m][1] + object[m][0] * 3;
			int ax = object[m][1] - object[m][3] * 3 / 2.0;
			int ay = object[m][0] - object[m][2] * 3 / 2.0;
			int bx = object[m][1] + object[m][3] * 3 / 2.0;
			int by = object[m][0] + object[m][2] * 3 / 2.0;

			printf("Object Center = %d \t ax = %d \t ay = %d \t bx = %d \t by = %d \t\n", object_center, ax, ay, bx, by);
			printf("Seed = %d \t Seed_x = %d \t Seed_y = %d \t\n\n", pixel, seed_x, seed_y);

			if (seed_x >= (ax - errorx) && seed_x <= (bx + errorx) && seed_y >= ay - (errory) && seed_y <= (by + errory)) {
				//seed is already in an object 
				seed_in_object = 1;
				printf("SEED IN OBJECT \n\n");
				break;
			}
		}

            // Check object size to target object parameters

            if (width >= WIDTH_MIN && width <= WIDTH_MAX && height >= HEIGHT_MIN && height <= HEIGHT_MAX) {

                objects_found++;

                printf("Hey! Shit's great %d, %d\n", up, down);

                // Calculate centers
                pixel        = Classifier::seed[j][1];
                int center_y = ((pixel - up * (Image::Width / 3)) + (pixel + down * (Image::Width / 3))) / 2;
                printf("Center y %d\n", center_y);
                center_y = (center_y / 3.0) / Image::Width;
                printf("Center y %d\n", center_y);
                int center_x = ((pixel - left) + (pixel + right)) / 2;
                printf("Center x %d\n", center_x);
                center_x = (center_x / 3.0) - center_y * Image::Width;
                printf("Center x %d\n", center_x);

			//cluster right
			int error = 0;

			while (error < 11) {
				rgb_colour.r = data[pixel] / 255.0;
				rgb_colour.g = data[pixel + 1] / 255.0;
				rgb_colour.b = data[pixel + 2] / 255.0;
				hsv_colour = rgb2hsv(rgb_colour);
				h_value = hsv_colour.h;
				s_value = hsv_colour.s * 255;
				v_value = hsv_colour.v * 255;

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {
					//printf("cluster right \n");
					//printf("pixel = {%d,%d,%d}\n", data[pixel], data[pixel + 1], data[pixel + 2]);
				}
				else {
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
				rgb_colour.r = data[pixel - 3] / 255.0;
				rgb_colour.g = data[pixel - 2] / 255.0;
				rgb_colour.b = data[pixel - 1] / 255.0;
				hsv_colour = rgb2hsv(rgb_colour);
				h_value = hsv_colour.h;
				s_value = hsv_colour.s * 255;
				v_value = hsv_colour.v * 255;

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {

					//printf("cluster left \n");
					//printf("pixel = {%d,%d,%d}\n", data[pixel-3], data[pixel -2], data[pixel -3]);
					left++;
					pixel = pixel - 3;
				}
				else {
					error++;
				}

				left++;

				//data_mod[pixel - 3] = { 255 };
				//data_mod[pixel - 2] = { 55 };
				//data_mod[pixel - 1] = { 125 };

				pixel = pixel - 3;
			}
			//printf("New up\n");
			//cluster up
			error = 0;
			pixel = seed[j][1];

			while (error < 11) {

				rgb_colour.r = data[pixel] / 255.0;
				rgb_colour.g = data[pixel + 1] / 255.0;
				rgb_colour.b = data[pixel + 2] / 255.0;
				hsv_colour = rgb2hsv(rgb_colour);
				h_value = hsv_colour.h;
				s_value = hsv_colour.s * 255;
				v_value = hsv_colour.v * 255;

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {
					//printf("cluster up \n");
					//printf("pixel = {%d,%d,%d}\n", data[pixel], data[pixel + 1], data[pixel + 2]);

					int p = (pixel / 3.0) / IMAGE_WIDTH;
					int q = (pixel / 3.0) - p *IMAGE_WIDTH;

					//printf("Pixel current %d, %d\n", q, p); 

					//printf("RGB up current= {{%d,%d,%d}\n\n", data[pixel], data[pixel + 1], data[pixel + 2]);

					//p = (pixel / 3.0) / IMAGE_WIDTH;
					//q = (pixel / 3.0) - p *IMAGE_WIDTH;

					//printf("Pixel new %d, %d\n", q, p);
					//printf("RGB up new= {{%d,%d,%d}\n\n", data[pixel], data[pixel + 1], data[pixel + 2]);
				}
				else {
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

				rgb_colour.r = data[pixel] / 255.0;
				rgb_colour.g = data[pixel + 1] / 255.0;
				rgb_colour.b = data[pixel + 2] / 255.0;
				hsv_colour = rgb2hsv(rgb_colour);
				h_value = hsv_colour.h;
				s_value = hsv_colour.s * 255;
				v_value = hsv_colour.v * 255;

				if (R_min <= h_value && R_max >= h_value
					&& G_min <= s_value && G_max >= s_value
					&& B_min <= v_value && B_max >= v_value
					) {

					//printf("cluster down \n");
					//printf("pixel = {%d,%d,%d}\n", data[pixel], data[pixel + 1], data[pixel + 2]);

					//printf("RGB down current = {{%d,%d,%d}\n", data[pixel], data[pixel + 1], data[pixel + 2]);

					//printf("RGB down new = {{%d,%d,%d}\n", data[pixel], data[pixel + 1], data[pixel + 2]);
				}
				else {
					error++;
				}
				down++;

				//data_mod[pixel + 0] = { 255 };
				//data_mod[pixel + 1] = { 55 };
				//data_mod[pixel + 2] = { 25 };

				pixel = pixel + IMAGE_WIDTH * 3;
			}

    double p = (Camera::resolution_x / (double) Image::Width) * Camera::pixel_x
               * (u - Image::Width / 2.0);  // The screen coordinates returned
    double q = (Camera::resolution_y / (double) Image::Height) * Camera::pixel_y
               * (v - Image::Height / 2.0);  // The screen coordinates returned

			if (width >= WIDTH_MIN && width <= WIDTH_MAX) {
				//printf("\nWIDTH IS GOOD\n\n");
				//printf("Seed = %d\n", j);
				//printf("pixel = %d\n", seed[j][1]);
				//int y = (seed[j][1] / 3.0) / IMAGE_WIDTH;
				//int x = (seed[j][1] / 3.0) - y *IMAGE_WIDTH;

				//printf("Pixel new %d, %d\n", q, p);
			}

    // Now convert the offsets into world coordinates
    // Vertical
    double plane_offset = 0;  // TODO Remove shitty hardcoded value

    double dist_x = (Kinematics::cam_height - plane_offset) * std::tan(M_PI_2 - theta - Kinematics::cam_phi);
    std::cout << "Distance x " << dist_x << std::endl;

    // Horizontal
    double dist_y =
        (Kinematics::cam_height - plane_offset) / std::cos(M_PI_2 - theta - Kinematics::cam_phi) * std::tan(phi);
    std::cout << "Distance y " << dist_y << std::endl;

    // Set the tank goal
    // These distances are relative to the tank, however i need them in world coordinates

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

				printf("Object %d, width %d, height %d, center x %d, center y %d\n", objects_found, width, height, center_x, center_y);
				printf("pixel = %d, left = %d, right = %d, y = %d, x = %d, up = %d, down = %d\n", pixel, left, right, y, x, up, down);

				int obj_center[2] = { center_x, center_y };
				Output_Segmentation(data_mod, IMAGE_WIDTH, IMAGE_HEIGHT, width, height, obj_center);

				//Find_distance(center_x, center_y);

				printf("Object found seed = %d\n", object[k][1]);

				k++;
			}
		}
	}
	printf("Objects found = %d \n", objects_found);



	/*printf("Saving file\n");
	std::ofstream outfile("Segmented_Image_3.ppm", std::ios::binary);
	outfile << "P6\n" << IMAGE_WIDTH << " " << IMAGE_HEIGHT << " 255\n";  // dont know
	outfile.write((char*)data_mod, (Image_Size));
	printf("Segmented_Image Saved\n");

	delete data_mod; */
	return;
}
