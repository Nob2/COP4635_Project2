#include "server.hpp"
#include <signal.h>

/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose:
*/

Server server;

void signal_callback_handler(int signum) {
        server.closeServer();
        exit(1);
}

int main() {
    // On Ctrl-C close the server and exit
    // Commented out thread joining, as if an active connection is still open the thread will not join
    signal(SIGINT, signal_callback_handler);

    server.initalizeServer();
    printf("Server is ready for requests.\n");
    server.handleRequests();
    
    return 0;
}