/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:21:54 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/06 15:31:28 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

        static bool                 signal ;    // track du signal
        int                         port;       // port d'ecoute
        int                         listen_fd;  // fd server
        std::vector<struct pollfd>  fds;        // vector de fds
        std::vector<Client>         clients;    // vector de clients
        struct sockaddr_in          CliAddr;    // socket client        
        struct sockaddr_in          sockAddr;   // socket server
        struct pollfd               Newpoll;    // structure du poll
        std::map<std::string, Channel>    channels; // map le nom des channel vers les obj Channel
        
    public:

        /* CONSTRUCTOR */
        Server();

        /* DESTRUCTOR */
        ~Server();

        void        ServerInit();
        void        Serverconnect();
        void        NewClient();
        void        ReceiveMessage(Client &client);
        void        handleReceivedMessage(char *buff, Client &client);
        void        CloseFds();
        void        ClearClients(int fd);
        static void SignalCatch(int signum);

        /* CHANNEL FUNCTIONS*/
        Channel * createChannel(std::string const &name);
        void deleteChannel(std::string const &name);
        Channel * launchChannel(std::string const &name);


};