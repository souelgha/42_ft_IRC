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
#include<signal.h>
#include"Client.hpp"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

class Server
{
    private:
        int port;  // port d ecoute
        int listen_fd; //fd server
        static bool signal ; // track du signal
        std::vector<struct pollfd> fds; //vector de fds
        std::vector<Client> clients; // vector de clients
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
        void ClearClients(int fd);
        static void SignalCatch(int signum);
};