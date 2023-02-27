/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose: Definition file for class clientManager
*/

#include "clientManager.hpp"

struct hostent* clientManager::retrieveHostName(std::string nameOfHost) {
    struct hostent *hostName = gethostbyname(nameOfHost);

    if (hostName == nullptr)
    {
        printf("Unable to determine hostName, exitting program\n");
        exit(1);
    }

    printf("Server IP is: %s\n", inet_ntoa(hostName->h_addr));

    return hostName;
}

void clientManager::connectToHost(std::string hostName) {
    struct hostent *hostName = retrieveHostName(hostName);

    this->socketAddress.sin_family = AF_INET;
    this->socketAddress.sin_port = htons(PORT_NUMBER);

    if((clientSocket = socket(AF_INET, SOCK_STREAM,0)) < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }

    int connectResult = connect(clientSocket, (struct sockaddr*) &server_address, sizeof(server_address));

    if(connectResult < 0) {
        printf("Failed to connect to the server\n");
        exit(1);
    }

    printf("Connection to server has succeed\n");
}

bool clientManager::retrieveFile(std::string fileName) {
    int result = send(clientSocket, fileName, strlen(fileName), 0);

    char buffer[1024] = {0};
    int readResult = read(clientSocket, buffer, sizeof(buffer));

    fileContents = std::string(buffer);

    //If less than 0, we failed to send our request or retrieve the file
    return !(result < 0 || readResult < 0);
}

void clientManager::printLastFile() {
    printf("%s\n\n", fileContents.c_str());
}

void clientManager::closeConnection() {
    close(clientSocket);
}