/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose: Definition file for class clientManager
*/

#include "clientManager.hpp"
#include <string.h>

struct hostent* clientManager::retrieveHostName(std::string nameOfHost) {
    this->nameOfHost = nameOfHost;
    struct hostent *hostName = gethostbyname(nameOfHost.c_str());

    if (hostName == nullptr)
    {
        printf("Unable to determine hostName, exitting program\n");
        exit(1);
    }

    server_address.sin_addr.s_addr = *((unsigned long *) hostName->h_addr);

    return hostName;
}

void clientManager::connectToHost(std::string hostName) {
    struct hostent *hostNameEnt = retrieveHostName(hostName);

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(PORT_NUMBER);

    if((clientSocket = socket(AF_INET, SOCK_STREAM,0)) < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address. Exiting program\n");
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

    std::string headerHead;
    std::string headerTail;

    headerHead = "GET " + fileName;
    headerTail = " HTTP/1.1 \n";
    headerTail += "Host: " + this->nameOfHost + "\n";
    headerTail += "Connection: keep-alive \n ";
    headerTail += "Accept: image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8 \n ";
    headerTail += "Sec-Fetch-Site: same-origin \n ";
    headerTail += "Sec-Fetch-Mode: no-cors \n ";
    headerTail += "Sec-Fetch-Dest: image \n ";
    headerTail += "Accept-Encoding: gzip, deflate, br \n ";
    headerTail += "Accept-Language: en-US,en;q=0.9 \n";

    std::string resultantRequest = headerHead;
    resultantRequest += headerTail;
    int result = send(clientSocket, resultantRequest.c_str(), strlen(fileName.c_str()), 0);

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