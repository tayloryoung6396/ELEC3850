/*
 * This should handle the initial processing of the images.
 * Inputs:  Camera Images - RGB
 * Outputs: Image in format required
 */

#include "Camera.hpp"

void Camera_init() {
    std::cout << "Initilising CAMERA" << std::endl;
//	raspicam::RaspiCam Camera;	
	if (!Camera.open()) {							//Error mesage if camera didnt open

		printf("Error: Camera could not open");
		return ;
	}

	delay(3000);									//Delay 3 seconds camera to stabalise
}

int Camera_main()
{
	//VARIABLES
	//unsigned char Red_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };	//May need to be divided by 3
	//unsigned char Green_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };
	//unsigned char Blue_data[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };

	unsigned char Red_data_array[IMAGE_HEIGHT*IMAGE_WIDTH] = { 0 };
	unsigned char Green_data_array[IMAGE_HEIGHT*IMAGE_WIDTH] = { 0 };
	unsigned char Blue_data_array[IMAGE_HEIGHT*IMAGE_WIDTH] = { 0 };

	//delay(3000);									//Delay 3 seconds camera to stabalise

	Camera.grab();									//Capture image 

	unsigned char *data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
	Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format

	for (i = 0; i <= IMAGE_HEIGHT*IMAGE_WIDTH;){
		for (j = 0; j <= IMAGE_HEIGHT*IMAGE_WIDTH / 3; j++) {

			Red_data_array[j] = data[i];
			i++;
			Green_data_array[j] = data[i];
			i++;
			Red_data_array[j] = data[i];
			i++;
		}
}

	/*for (int Height = 0; Height <= IMAGE_HEIGHT; Height++) {

		for (int Width = 0; Width <= IMAGE_WIDTH;) {

			Red_data[Height][Width] = data[Height*Width];

			Width++;

			Green_data[Height][Width] = data[Height*Width];

			Width++;

			Blue_data[Height][Width] = data[Height*Width];

			Width++;
		}
		 */
	}

	//Camera.retrieve(Red_data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
	std::ofstream outFile("Red_Image.ppm", std::ios::binary); //Save Image
	outFile << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";									//dont know
	outFile.write((char*)Red_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
	printf("Red Image Saved");

	//Camera.retrieve(Green_data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
	std::ofstream outFile1 ("Green_Image.ppm", std::ios::binary); //Save Image
	outFile1 << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";									//dont know
	outFile1.write((char*)Green_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
	printf("Green Image Saved");

	//Camera.retrieve(Blue_data, raspicam::RASPICAM_FORMAT_RGB); //Extract image in rgb format
	std::ofstream outFile2 ("Blue_Image.ppm", std::ios::binary); //Save Image
	outFile2 << "P6\n" << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";									//dont know
	outFile2.write((char*)Blue_data_array, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
	printf("Blue Image Saved");

	return 0;
