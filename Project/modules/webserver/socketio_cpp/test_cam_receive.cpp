#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void error(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno                    = atoi(argv[1]);
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(portno);
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
    listen(sockfd, 5);
    clilen    = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");
    bzero(buffer, 1024);

    Mat img     = Mat::zeros(height, width, CV_8UC3);
    int imgSize = img.total() * img.elemSize();
    uchar sockData[imgSize];

    // Receive data here

    for (int i = 0; i < imgSize; i += bytes) {
        if ((bytes = recv(connectSock, sockData + i, imgSize - i, 0)) == -1) {
            quit("recv failed", 1);
        }
    }

    // Assign pixel value to img

    int ptr = 0;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
            ptr                     = ptr + 3;
        }
    }

    close(newsockfd);
    close(sockfd);

    namedWindow("Server", CV_WINDOW_AUTOSIZE);  // Create a window for display.
    imshow("Server", image);
    waitKey(0);
    return 0;
}