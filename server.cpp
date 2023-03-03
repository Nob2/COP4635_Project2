#include "server.hpp"
#include <fstream>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>

/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose: Definition file for class Server
 */

void Server::defineSocketAddress()
{
    this->socketAddress.sin_family = AF_INET;
    this->socketAddress.sin_port = htons(PORT_NUMBER);
    this->lengthOfAddress = sizeof(this->socketAddress);

    struct hostent *hostName = gethostbyname("localhost");

    if (hostName == nullptr)
    {
        printf("Unable to determine hostName, exitting program\n");
        exit(1);
    }

    this->socketAddress.sin_addr.s_addr = *((unsigned long *)hostName->h_addr);

    int bindNumber = bind(listeningSocket, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress));

    if (bindNumber == -1)
    {
        printf("Error binding the socket, exitting server\n");
        exit(1);
    }

    printf("I will be utilizing the IP address of: %s\n", inet_ntoa(socketAddress.sin_addr));
}

void Server::addFilesToMap()
{
    avaialableFiles["img.jpg"] = "img.jpg";
    avaialableFiles["index.html"] = "index.html";
    avaialableFiles["testPresence.html"] = "testPresence.html";
}

void Server::initalizeListenSocket()
{
    this->listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listeningSocket == -1)
    {
        printf("Error creating initial socket, exitting program\n");
        exit(1);
    }
}

void Server::beginListening()
{
    int listenResponse = listen(listeningSocket, 5);
    if (listenResponse == -1)
    {
        printf("Error listening for requests, exitting\n");
        exit(1);
    }
}

void Server::initalizeServer()
{

    addFilesToMap();
    initalizeListenSocket();
    defineSocketAddress();
    beginListening();
}

void Server::handleIndividualRequest(int socket)
{
    std::string requiredFile = parseForFile(socket);

    const char *fileName = requiredFile.c_str();
    bool isImage = (requiredFile == "img.jpg" ? true : false);

    if (isImage)
        sendImage(socket, fileName);
    else
        sendDocument(socket, fileName);

    printf("Responded to client\n");
    close(socket);
}

std::string Server::parseForFile(int socket)
{
    // Headers needed to determine file name
    std::string index = "GET / HTTP/1.1";
    std::string presence = "testPresence.html";
    std::string image = "img.jpg";
    char buffer[1024] = {0};
    int socketRead;

    socketRead = read(socket, buffer, 1024);
    if (socketRead == -1)
    {
        printf("Error communicating to socket %d, closing connection", socket);
        close(socketRead);
        return "";
    }
    std::string stringBuffer(buffer);

    std::istringstream clientRequest(stringBuffer);
    std::string documentName = "";

    //First line is the document being requested
    std::getline(clientRequest, documentName);
    close(socketRead);

    if (documentName.find(index) != std::string::npos)
        documentName = "index.html";
    else if (documentName.find(image) != std::string::npos)
        documentName = image;
    else if (documentName.find(presence) != std::string::npos)
        documentName = presence;
    
    return documentName;
}

void Server::sendImage(int socket, const char fileName[])
{
    std::string header = "HTTP/1.1 200 Okay\r\nContent-Type: image/jpg; Content-Transfer-Encoding: binary; Content-Length: 65420; charset=ISO-8859-4 \r\n\r\n";

    FILE *image = fopen(fileName, "rb");
    if (image == nullptr)
    {
        printf("Failed to open image for client\n");
        char message[] = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>IMAGE NOT FOUND</h1>";
        send(socket, message, strlen(message), 0);

        return;
    }

    fseek(image, 0, SEEK_END);
    unsigned long fileSize = ftell(image);

    char *buffer = (char *)malloc(sizeof(char) * fileSize);
    rewind(image);

    fread(buffer, sizeof(char), fileSize, image);

    const char *finalHeader = header.c_str();

    send(socket, finalHeader, strlen(finalHeader), 0);
    send(socket, buffer, fileSize, 0);
}

void Server::sendDocument(int socket, const char fileName[])
{
    if (!avaialableFiles.count(fileName))
    {
        printf("Failed to open file: %s for client\n", fileName);
        char message[] = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>FILE NOT FOUND</h1>";
        send(socket, message, strlen(message), 0);
    }
    else
    {
        std::string fileContents;
        std::ifstream input;

        input.open(fileName);
        fileContents = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n";

        // Read the contents of the file and place it into fileContents
        while (!input.eof())
        {
            std::string tempBuffer;
            std::getline(input, tempBuffer);
            fileContents += tempBuffer + "\n";
        }

        // Send message, close input file
        const char *finalMessage = fileContents.c_str();
        send(socket, finalMessage, strlen(finalMessage), 0);
        input.close();
    }
}

void Server::handleRequests()
{
    while (true)
    {
        int communicationSocket = accept(listeningSocket, NULL, NULL);

        if (communicationSocket == -1)
        {
            printf("Error accepting connection\n");
            exit(1);
        }

        this->clientThreads.push_back(std::thread(&Server::handleIndividualRequest, this, communicationSocket));
    }
}

void Server::closeServer()
{
    /** for (size_t i = 0; i < this->clientThreads.size(); i++)
    {
        this->clientThreads.at(i).join();
    } **/
    shutdown(listeningSocket, SHUT_RDWR);
}