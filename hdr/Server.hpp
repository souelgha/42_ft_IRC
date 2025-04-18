/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 12:44:16 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/24 16:26:59 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <signal.h>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>

#include "RPL_ERR.hpp"
#include "Client.hpp"
#include "Channel.hpp"

/* COLORS */
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

        static size_t const                 maxClients = 100;
        static bool                         signal;                 // track du signal

        std::string const                   name;
        std::string const                   password;               // mot de passe pour se connecter au serveur

        int                                 listen_port;            // port d'ecoute
        int                                 listen_fd;              // fd server
        std::vector<struct pollfd>          fds;                    // liste des fds
        struct sockaddr_in                  listenAddress;          // socket d'ecoute
        struct sockaddr_in                  clientAddress;          // socket client        
        Client                              *clients[maxClients];   // liste des clients
        Client                              *toDelete[maxClients];  // liste des clients
        std::map<std::string, Channel *>    channels;               // map le nom des channel vers les obj Channel

        /* CONSTRUCTOR */
        Server(void);

        /* UTILS */
        void                                addClient(Client *client);
        void                                addToDelete(Client *client);
        void                                deleteClients(void);
        void                                deleteChannel(Channel *channel);

    public:

        /* CONSTRUCTOR */
        Server(int listening_port, std::string const &password);

        /* DESTRUCTOR */
        ~Server(void);

        /* GETTERS */
        size_t                              getMaxClients(void) const ;
        std::string const                   &getName(void) const ;
        std::string const                   &getPassword(void) const ;
        int                                 getListenPort(void) const ;
        Client                              **getClients(void) ;
        std::map<std::string, Channel *>    &getChannels(void) ;

        void                                serverInit(void);
        void                                serverConnect(void);
        void                                newClient(void);
        void                                receiveMessage(Client *client);

        /* UTILS */
        static void                         signalCatch(int signum);
        void                                clearClient(Client *client);
        bool                                isClient(std::string const &nickname);
        Client                              *findClient(std::string const &name);
        Client                              *findClient(int fd);
        void                                createChannel(Client const &client, std::string const &name, std::string const &key);

        /* REPLIES */
        void                                sendTemplate(Client const &client, std::string const &message);
        void                                replyNick(Client &client, std::string const &newnick);
        void                                replyQuit(Client &client, std::string const &reason);
        void                                replyPrivmsgClient(Client const &sender, Client const &recipient, std::string const &toSend);
        void                                replyPrivmsgChannel(Client const &client, Channel &channel, std::string const &toSend);
        void                                replyJoin(Client const &client, Channel &channel);
        void                                replyPart(Client &client, Channel *channel);
        // void                                replyPart(Client const &client, Channel *channel);
        void                                replyKick(Client const &client, Channel *channel, Client *recipient, std::string const &reason);
        void                                replyInvite(Client const &sender, Client const &recipient, Channel &channel);
        void                                replyTopic(Client const &client, Channel &channel, std::string const &topic);
        void                                replyModeClient(Client const &client);
        void                                replyModeChannel(Client const &client, Channel &channel, std::string const &mode);
        void                                replyWho(Client const &client, Channel &channel);
        void                                replyPing(Client const &client, std::string const &pong);
        void                                replyUnknown(Client const &client, std::string const &command);
};
