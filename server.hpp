#ifndef SERVER_HPP
#define SERVER_HPP

#define PORT_NUMBER 60001
#define HTTP_PORT 443

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include <vector>
#include <unordered_map>
#include <string>
#include <string.h>

/**
 * Author: Nicholas Brown-Duncan
 * Course: COP4635 Project 2
 * Date: 02/26/23
 * Purpose: Header file for class Server.
 * Server is responsible for creating the intiial main socket that allows clients to communicate with us,
 * alongside spinning up threads upon client connection. 
*/

class Server {
    private:
        int listeningSocket;
        int lengthOfAddress;
        struct sockaddr_in socketAddress;
        std::vector<std::thread> clientThreads;
        std::unordered_map<std::string, std::string> avaialableFiles;

        void defineSocketAddress();
        void initalizeListenSocket();
        void beginListening();
        void addFilesToMap();
        void handleIndividualRequest(int socket);
        void sendDocument(int socket, const char fileName[]);
        std::string parseForFile(int socket);
        void sendImage(int socket, const char fileName[]);
    public:
        void initalizeServer();
        void handleRequests();
        void closeServer();
};


#endif