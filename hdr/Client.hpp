/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:27 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/10 12:51:19 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "Server.hpp"
#include "RPL_ERR.hpp"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 512
#endif

#ifndef DELIMITER
# define DELIMITER "\r\n"
#endif

class   Server;

class   Client
{
    private:

        int             fd;                     // fd du client
        std::string     ipAddress;              // adresse socket client

        bool            authentification;

        std::string     realName;
        std::string     hostName;
        std::string     userName;
        std::string     nickName;
        std::string     serverName;
        std::string     sourceName;
        std::string     mode;

    public:

        char            buffer[BUFFER_SIZE];    // buffer personnel du client

        /* CONSTRUCTORS */
        Client(void);

        /* DESTRUCTORS */
        ~Client(void);

        /* GETTERS */
        int                 getFd() const;
        std::string         getIpAddress() const;
        std::string const   &getRealName() const;
        std::string const   &getHostName() const;
        std::string const   &getUserName() const;
        std::string         getNickName() const; // mod sonia
        std::string const   &getServerName() const;
        std::string const   &getSourceName() const;
        std::string const   &getMode() const ;

        /* SETTERS */
        void                setFd(int fd);
        void                setIpAddress(std::string addr);
        void                setRealName(std::string const &realName);
        void                setHostName(std::string const &hostName);
        void                setUserName(std::string const &userName);
        void                setNickName(std::string const &nickName);
        void                setServerName(std::string const &serverName);
        void                setMode(std::string const &mode);
        void                setSourceName(void);

        void                commandReact(Server &server);
        std::string         extractMessage(std::string const &buffer);
        void                handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter);
        bool                commandConnect(Server &server);

        /* COMMANDS */
        void                commandPass(Server &, std::string const &parameter);
        void                commandNick(Server &, std::string const &parameter);
        void                commandUser(Server &server, std::string const &parameter);
        void                commandMode(Server &server, std::string const &parameter);
        void                commandQuit(Server &server, std::string const &parameter);
        void                commandWhois(Server &server, std::string const &);
        void                commandPing(Server &server, std::string const &parameter);
        void                commandPrivmsg(Server &server, std::string const &parameter);


        /* CHANNEL COMMANDS */
        void                commandJoin(Server &server, std::string const &parameter);
        void                commandPart(Server &server, std::string const &parameter);
};
