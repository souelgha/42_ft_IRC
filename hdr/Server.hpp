#pragma once

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <signal.h>
#include <errno.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#include "Client.hpp"
#include "Channel.hpp"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 512
#endif

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 10
#endif

#ifndef DELIMITER
# define DELIMITER "\r\n"
#endif

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

class Client;

class Server
{
    private:

        static bool                 signal ;        // track du signal
        int                         listen_port;    // port d'ecoute
        int                         listen_fd;      // fd server
        std::vector<struct pollfd>  fds;            // liste des fds
        std::vector<Client>         clients;        // liste des clients
        struct sockaddr_in          listenAddress;  // socket d'ecoute
        struct sockaddr_in          clientAddress;  // socket client        
        std::map<std::string, Channel>    channels; // map le nom des channel vers les obj Channel
        std::vector<std::string>        listChannel; // liste des channels sur le server
        
    public:

        /* CONSTRUCTOR */
        Server(void);

        /* DESTRUCTOR */
        ~Server(void);

        void        serverInit(void);
        void        serverConnect(void);
        void        newClient(void);
        void        ReceiveMessage(Client &client);
        void        handleReceivedMessage(char *buff, Client &client);
        void        sendAnswer(std::string const &command, Client &client);
        void        closeFds();
        void        clearClients(int fd);
        std::string getCommand(std::string message);
        std::string getRealName(std::string message);
        void        commandNick(std::string const &message, Client &client);
        void        commandUser(std::string const &message, Client &client);
        void        commandMode(std::string const &message);
        void        commandWhois(std::string const &message);
        void        commandPing(std::string const &message, Client &client);
        void        answerUser(Client &client);
        void        answerMode(Client &client);
        void        answerWhois(Client &client);
        void        answerPing(Client &client, std::string const &ping);
        static void SignalCatch(int signum);

        /* CHANNEL FUNCTIONS*/
        void deleteChannel(std::string const &name);
        Channel &getChannel(std::string const &nickname, std::string const &name);
        std::vector<std::string> followlistChannels();
        void HandleJoinCommand(std::string const &nickname, std::string const &chanelname);
        void sendToClient(std::string const &nickname, std::string const &message);
 
};