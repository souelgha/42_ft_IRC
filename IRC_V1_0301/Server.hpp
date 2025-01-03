#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include <poll.h>
#include<vector>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

class Server
{
    private:
        int port;
        int listen_fd;
        std::vector<struct pollfd> fds;


    public:
        Server();
        ~Server();
        void ServerInit();
        void Serverconnect();
        void NewClient();
        void ReceiveMessage();

};