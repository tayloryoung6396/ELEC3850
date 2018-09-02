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
    exit(0);
}

int main(int argc, char* argv[]) {
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

    Mat frame;
    frame = (frame.reshape(0, 1));  // to make it continuous

    int imgSize = frame.total() * frame.elemSize();

    // Send data here
    bytes = send(clientSock, frame.data, imgSize, 0);
    close(sockfd);

    namedWindow("Client", CV_WINDOW_AUTOSIZE);  // Create a window for display.
    imshow("Client", image);
    waitKey(0);
    return 0;
}
