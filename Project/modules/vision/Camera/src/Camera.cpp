/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "Camera.hpp"

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;
    //	raspicam::RaspiCam Camera;
    if (!Camera.open()) {  // Error mesage if camera didnt open

        printf("Error: Camera could not open\n");
        return;
    }
	Camera.setHorizontalFlip(1);
	Camera.setVerticalFlip(1);

    delay(3000);  // Delay 3 seconds camera to stabalise
    // sleep(3);
    // capture
   // Camera.grab();
    // allocate memory
  //  unsigned char* data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
    // extract the image in rgb format
    //Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // get camera image
    // save
//    std::ofstream outFile("raspicam_image.ppm", std::ios::binary);
  //  outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
  //  outFile.write((char*) data, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
  //  std::cout << "Image saved at raspicam_image.ppm" << std::endl;
    // free resrources
  //  delete data;
}

int Camera_main() {
    // VARIABLES
    unsigned char Red_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };	//May need to be divided by 3
    // unsigned char Green_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };
    // unsigned char Blue_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };

     //unsigned char Red_data_array[IMAGE_HEIGHT * IMAGE_WIDTH]   = {0};
unsigned char* Red_data_array = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];    
// unsigned char Green_data_array[IMAGE_HEIGHT * IMAGE_WIDTH] = {0};
    // unsigned char Blue_data_array[IMAGE_HEIGHT * IMAGE_WIDTH]  = {0};

     delay(3000);									//Delay 3 seconds camera to stabalise

     Camera.grab();  // Capture image
    // printf("before data\n");
     unsigned char* data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
    // printf("Before retrieve\n");
     Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);  // Extract image in rgb format
    // printf("After retrieve\n");
//     for (int i = 0; i <= IMAGE_HEIGHT * IMAGE_WIDTH;) {
  //       for (int j = 0; j <= IMAGE_HEIGHT * IMAGE_WIDTH / 3; j++) {

    //         Red_data_array[j] = data[i];
      //      i++;
        //    Green_data_array[j] = data[i];
      //      i++;
    //       Red_data_array[j] = data[i];
        //   i++;
     //    }
   //  }
	 int i = 0;
     for (int Height = 0; Height < IMAGE_HEIGHT; Height++) {

       for (int Width = 0; Width < IMAGE_WIDTH;) {
//			 std::cout << "Data " << (int) data[0] << std::endl; 
    		         Red_data[Height][Width] = data[2*Height+Width];
printf("Red data %d \n",Red_data[Height][Width]); 
			 Red_data_array[i] = Red_data[Height][Width];
printf("Array %d \n", Red_data_array[i]);			 
Red_data[Height][Width + 1] = 0;
			 Red_data[Height][Width + 2] = 0;
			// Red_data_array[i] = Red_data[Height][Width+2];
			 i+=2;

			 Width++;

	/*		 Green_data[Height][Width-1] = 0;
			 Green_data[Height][Width] = data[Height*Width];
			 Green_data[Height][Width + 1] = 0;
    */

             Width++;
	/*		 Blue_data[Height][Width - 2] = 0;
			 Blue_data[Height][Width - 1] = 0;
			 Blue_data[Height][Width] = data[Height*Width];
*/
             Width++;
        }

    }

//	 for 

printf("Saving file\n");
     //Camera.retrieve(Red_data_array, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
     std::ofstream outfile("Red_Image.ppm", std::ios::binary);                         // Save Image
     outfile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";  // dont know
     outfile.write((char*) Red_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
     printf("Red Image Saved\n");

    // // Camera.retrieve(Green_data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
    // // std::ofstream outFile1 ("Green_Image.ppm", std::ios::binary); //Save Image
    // // outFile1 << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
    // //dont
    // // know outFile1.write((char*)Green_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    // // printf("Green Image Saved");

    // // Camera.retrieve(Blue_data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
    // // std::ofstream outFile2 ("Blue_Image.ppm", std::ios::binary); //Save Image
    // // outFile2 << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
    // //dont
    // // know outFile2.write((char*)Blue_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
    // printf("Blue
    // // Image Saved");

    return 0;
}
