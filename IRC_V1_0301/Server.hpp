#pragma once

#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include <poll.h>
#include<vector>
#include"Client.hpp"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10


class Server
{
    private:
        int port;  // port d ecoute
        int listen_fd; //fd server
        std::vector<struct pollfd> fds; //vector de fds
        std::vector<Client> client; // vector de clients
        struct sockaddr_in CliAddr; //socket client        
        struct sockaddr_in sockAddr; // socket server
        struct pollfd   Newpoll; //structure du poll
        


    public:
        Server();
        ~Server();
        void ServerInit();
        void Serverconnect();
        void NewClient();
        void ReceiveMessage(int fd);
        void CloseFds();
};