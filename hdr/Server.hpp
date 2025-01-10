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
#include "Error.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 10
#endif

#ifndef LISTENING_PORT
# define LISTENING_PORT 6697
#endif

#ifndef PASSWORD
# define PASSWORD "password"
#endif

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

class   Client;
class   Channel;

class   Server
{
    private:

        static bool                     signal ;        // track du signal
        int                             listen_port;    // port d'ecoute
        int                             listen_fd;      // fd server
        std::vector<struct pollfd>      fds;            // liste des fds
        std::vector<Client>             clients;        // liste des clients
        struct sockaddr_in              listenAddress;  // socket d'ecoute
        struct sockaddr_in              clientAddress;  // socket client        
        std::map<std::string, Channel>  channels;       // map le nom des channel vers les obj Channel
        std::vector<std::string>        listChannel;    // liste des channels sur le server
        
    public:

        /* CONSTRUCTOR */
        Server(void);

        /* DESTRUCTOR */
        ~Server(void);

        /* GETTERS */
        int                             getListenPort(void) const ;
        std::vector<Client>             getClients(void) const ;
        std::map<std::string, Channel>  &getChannels(void) ;

        void                            serverInit(void);
        void                            serverConnect(void);
        void                            newClient(void);
        void                            receiveMessage(Client &client);
        void                            closeFds(void);
        void                            clearClient(int fd);
        static void                     SignalCatch(int signum);
        Client                         &findClient(std::string const &name);

        /* REPLIES */
        void                            replyUser(Client &client);
        void                            replyMode(Client &client);
        void                            replyQuit(Client &client, std::string const &reason);
        void                            replyWhois(Client &client);
        void                            replyPing(Client &client, std::string const &pong);
        void                            replyJoin(Client &client, Channel &channel);
        void                            replyPart(Client &client, Channel &channel);
        void                            replyPrivmsgClient(Client &sender, Client &recipient, std::string const &toSend);
        // void                            replyPrivmsgChannel(Client &client, Channel &channel, std::string const &toSend);


        /* CHANNEL FUNCTIONS*/
        Channel                         &findChannel(Client &client, std::string const &name);
        std::vector<std::string>        followlistChannels(void); 
        void                            deleteChannel(std::string const &name);
};