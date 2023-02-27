#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose: Client to allow command-line communication to the Server. Serves in lieu of when unable to properly use a browser client.
*/

#include "clientManager.hpp"
#include <iostream>

int main(int argv, char** argc) {
    if(argv < 2) {
        printf("Must specify the name of the server\n");
        return 1;
    }

    clientManager manager;
    manager.connectToHost(argc[1]);

    std::string fileName;

    while(true) {
        printf("What is the name of the file you wish to request? Enter quit to exit ");
        std::cin >> fileName;

        if(fileName == "quit")
            break;

        bool successfulTransmission = manager.retrieveFile(fileName);
        if(successfulTransmission)
            manager.printLastFile();
        else {
            printf("Failed to send request or retrieve file, exitting program\n");
            manager.closeConnection();
            exit(1);
        }

        // Server is set up to close connection after each request
        manager.closeConnection();
        manager.connectToHost(argc[1]);
    }

    manager.closeConnection();

    return 0;
}