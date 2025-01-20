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
#include <fcntl.h>
#include <poll.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "RPL_ERR.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 10
#endif

#ifndef LISTENING_PORT
# define LISTENING_PORT 6697
#endif

#ifndef PASSWORD
# define PASSWORD "42"
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
        struct sockaddr_in              listenAddress;  // socket d'ecoute
        struct sockaddr_in              clientAddress;  // socket client        
        std::vector<Client>             clients;        // liste des clients
        std::map<std::string, Channel>  channels;       // map le nom des channel vers les obj Channel
        // std::vector<std::string>        listChannel;    // liste des channels sur le server
        
    public:

        /* CONSTRUCTOR */
        Server(void);

        /* DESTRUCTOR */
        ~Server(void);

        /* GETTERS */
        int                             getListenPort(void) const ;
        std::vector<Client>             &getClients(void) ;
        std::map<std::string, Channel>  &getChannels(void) ;

        void                            serverInit(void);
        void                            serverConnect(void);
        void                            newClient(void);
        void                            receiveMessage(Client &client);

        /* UTILS */
        void                            closeFds(void);
        void                            clearClient(int fd);
        static void                     signalCatch(int signum);
        bool                            isClient(std::string const &nickname);
        Client                          &findClient(std::string const &name);


        /* REPLIES */
        void                            replyNick(Client &client, std::string const &newnick);
        void                            replyUser(Client const &client);
        void                            replyModeClient(Client const &client);
        void                            replyModeChannel(Client const &client, Channel &channel, std::string const &mode);
        void                            replyQuit(Client &client, std::string const &reason);
        void                            replyJoin(Client const &client, Channel &channel);
        void                            replyPart(Client const &client, Channel &channel);
        void                            replyPrivmsgClient(Client const &sender, Client const &recipient, std::string const &toSend);
        void                            replyPrivmsgChannel(Client const &client, Channel &channel, std::string const &toSend);
        void                            replyKick(Client const &client, Channel &channel, Client const &recipient, std::string const &reason);
        void                            replyInvite(Client const &sender, Client const &recipient, Channel &channel);
        void                            replyTopic(Client const &client, Channel &channel, std::string const &topic);
        void                            replyWho(Client const &client, Channel &channel);
        // void                            replyWhoIs(Client const &client);
        void                            replyPing(Client const &client, std::string const &pong);
       
       /* REPLY ERRORS */
        void                            replyErrRegistered(Client &client);
        void                            replyWrongPwd(Client &client);
        void                            replyErrNick(Client &client);
        void                            replyErronNickUse(Client &client);
        void                            replyMissPara(Client &client, std::string &command);
        void                            sendTemplate(Client const &client, std::string const &message);
        // void                            replyWrongConnect(Client &client);
        void                            replyUnknown(Client const &client, std::string const &command);

        /* CHANNEL FUNCTIONS*/
        // void                            createChannel(Client const &client, std::string const &name);
        void                            createChannel(Client const &client, std::string const &name, std::string const &key);
        // Channel                         &findChannel(std::string const &name);
        void                            deleteChannel(std::string const &name);
        void                            clearChannels(void);
};