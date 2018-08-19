#include "bluetoothscan.h"

int main(int argc, char** argv) {
    inquiry_info* ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19]  = {0};
    char name[248] = {0};

    dev_id = hci_get_route(NULL);
    sock   = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }
