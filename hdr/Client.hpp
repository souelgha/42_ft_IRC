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
class   Channel;

class   Client
{
    private:

        int             fd;                     // fd du client
        std::string     ipAddress;              // adresse socket client

        bool            pass_command;
        bool            authentification;

        std::string     realName;
        std::string     hostName;
        std::string     userName;
        std::string     nickName;
        std::string     serverName;
        std::string     sourceName;
        std::string     mode;

        std::string         extractMessage(std::string const &buffer);
        std::vector< Channel *> listchannel;

    public:

        char            buffer[BUFFER_SIZE];    

        /* CONSTRUCTORS */
        Client(void);

        /* DESTRUCTORS */
        virtual ~Client(void);

        /* GETTERS */
        int                 getFd(void) const;
        std::string const   &getIpAddress(void) const;
        std::string const   &getRealName(void) const;
        std::string const   &getHostName(void) const;
        std::string const   &getUserName(void) const;
        std::string         getNickName(void) const; 
        std::string const   &getServerName(void) const;
        std::string const   &getSourceName(void) const;
        std::string const   &getMode(void) const;
        bool                getAuthentification(void) const;
        std::vector<Channel>  &getListChannel(void) const;

        /* SETTERS */
        void                setFd(int fd);
        void                setIpAddress(std::string const &addr);
        void                setRealName(std::string const &realName);
        void                setHostName(std::string const &hostName);
        void                setUserName(std::string const &userName);
        void                setNickName(std::string const &nickName);
        void                setServerName(std::string const &serverName);
        void                setSourceName(void);
        void                setMode(std::string const &mode);


        void                commandReact(Server &server);
        void                handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter);

        /* COMMANDS */
        void                commandPass(Server &, std::string const &parameter);
        void                commandNick(Server &, std::string const &parameter);
        void                commandUser(Server &server, std::string const &parameter);
        void                commandMode(Server &server, std::string const &parameter);
        void                commandQuit(Server &server, std::string const &parameter);
        void                commandPrivmsg(Server &server, std::string const &parameter);
        void                commandWho(Server &server, std::string const &);
        void                commandPing(Server &server, std::string const &parameter);
        void                commandUnknown(Server &server, std::string const &parameter);
        void                commandCap(Server &, std::string const &) ;

        /* CHANNEL COMMANDS */
        void                commandJoin(Server &server, std::string const &parameter);
        void                commandPart(Server &server, std::string const &parameter);
        void                commandKick(Server &server, std::string const &parameter);
        void                commandInvite(Server &server, std::string const &parameter);
        void                commandTopic(Server &server, std::string const &parameter);
};
