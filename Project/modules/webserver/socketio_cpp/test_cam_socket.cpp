// g++ simpletest_raspicam_cv.cpp -o  simpletest_raspicam_cv -I/usr/local/include/ -lraspicam -lraspicam_cv -lmmal
// -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui

#include <netdb.h>
#include <netinet/in.h>
#include <raspicam/raspicam_cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

int main(int argc, char** argv) {

    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    int nCount = 1;
    // set camera params
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
    // Open camera
    cout << "Opening Camera..." << endl;
    if (!Camera.open()) {
        cerr << "Error opening the camera" << endl;
        return -1;
    }
    // Start capture
    cout << "Capturing " << nCount << " frames ...." << endl;
    for (int i = 0; i < nCount; i++) {
        Camera.grab();
        Camera.retrieve(image);
        if (i % 5 == 0) cout << "\r captured " << i << " images" << std::flush;
    }
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting");

    image = (image.reshape(0, 1));  // to make it continuous

    int imgSize = image.total() * image.elemSize();

    // Send data here
    bytes = send(clientSock, image.data, imgSize, 0);
    close(sockfd);

    namedWindow("Client", CV_WINDOW_AUTOSIZE);  // Create a window for display.
    imshow("Client", image);
    Camera.release();
}
