/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/27 10:55:34 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : fd(-1), pass_command(false), authentification(false), registered(false) {

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
}

Client::~Client(void) {

    if (this->fd != -1)
        close (this->fd);
}

int Client::getFd(void) const {

    return(this->fd);
}

std::string const &Client::getIpAddress(void) const {

    return(this->ipAddress);
}

std::string const   &Client::getRealName(void) const {

    return(this->realName);
}

std::string const   &Client::getHostName(void) const {

    return(this->hostName);
}

std::string const   &Client::getUserName(void) const {

    return(this->userName);
}

std::string Client::getNickName(void) const {

    return(this->nickName);
}

bool  Client::getAuthentification(void) const {
    
    return(this->authentification);
}

std::vector<Channel *>  &Client::getListChannels(void) {

    return (this->listChannels);
}

std::string const   &Client::getServerName(void) const {

    return(this->serverName);
}

std::string const   &Client::getSourceName(void) const {

    return(this->sourceName);
}

std::string const   &Client::getMode(void) const {

    return(this->mode);
}

void    Client::setFd(int fd) {

    this->fd = fd;
}

void    Client::setIpAddress(std::string const &address) {

    this->ipAddress = address;
}

void    Client::setRealName(std::string const &realName) {

    this->realName = realName;
}

void    Client::setHostName(std::string const &hostName) {

    this->hostName = hostName;
}

void    Client::setUserName(std::string const &userName) {

    this->userName = userName;
}

void    Client::setNickName(std::string const &nickName) {

    this->nickName = nickName;
}

void    Client::setServerName(std::string const &serverName) {

    this->serverName = serverName;
}
void    Client::setSourceName(void) {

    std::string sourceName =
        this->nickName + "!" + this->userName + "@" + this->hostName;
    this->sourceName = sourceName;
}

void    Client::setMode(std::string const &mode) {

    this->mode = mode;
}

std::string const   Client::extractLast(std::string const &parameter) {

    if (parameter.empty())
        return ("");
    size_t  i = parameter.find(':');
    if (i == std::string::npos)
        return (parameter);

    std::string reason = parameter.substr(i + 1, parameter.length() - i);
    return (reason);
}

std::string Client::extractMessage(std::string const &buffer) {

    std::string message;
    size_t      end;
    std::string remainder;

    end = buffer.find(DELIMITER);
    message = buffer.substr(0, end);
    remainder = buffer.substr(end + std::strlen(DELIMITER), buffer.length() - (end + std::strlen(DELIMITER)));
    std::fill(this->buffer, this->buffer + BUFFER_SIZE, 0);
    if (!remainder.empty())
        std::memcpy(this->buffer, remainder.c_str(), remainder.length());
    return (message);
}

static std::string  extractPrefix(std::string &message) {

    std::istringstream  split(message);
    std::string         prefix = "";

    if (message[0] == ':')
        split >> prefix;
    std::getline(split >> std::ws, message);
    return (prefix);
}

static std::string  extractCommand(std::string &message) {

    std::istringstream  split(message);
    std::string         command;

    split >> command;
    std::getline(split >> std::ws, message);
    if (split.fail())
        message.clear();
    return (command);
}

void    Client::commandReact(Server &server) {

    std::string buffer = this->buffer;
    std::string message;
    std::string prefix;
    std::string command;
    std::string parameter;

    while (!buffer.empty() && buffer.find(DELIMITER) != std::string::npos)
    {
        message = this->extractMessage(buffer);
        std::cout
            << "<< " << message << std::endl;

        prefix = extractPrefix(message);
        command = extractCommand(message);
        parameter = message;
        try {
            this->handleCommand(server, prefix, command, parameter);
        }
        catch (std::exception &e) {
            throw ;
        }
        buffer = this->buffer;
    }
}

void    Client::handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter) {
    
  
    void        (Client::*actions[14])(Server &, std::string const &) =
        {&Client::commandPass, &Client::commandNick, &Client::commandUser, &Client::commandQuit,
        &Client::commandPrivmsg, &Client::commandJoin, &Client::commandPart,
        &Client::commandKick, &Client::commandInvite, &Client::commandTopic, &Client::commandMode,
        &Client::commandWho, &Client::commandPing, &Client::commandCap};
    std::string sent[] = {"PASS", "NICK", "USER", "QUIT", "PRIVMSG", "JOIN", "PART", "KICK", "INVITE", "TOPIC", "MODE", "WHO", "PING", "CAP"};
    int         i;

    for (i = 0; i < 14; i++)
    {
        if (command == sent[i])
        {
            try {
                (this->*actions[i])(server, parameter);
                return ;
            }
            catch (std::exception &e) {
                throw ;
            }
        }
    }
    try {
        this->commandUnknown(server, command);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandPass(Server &server, std::string const &parameter) {

    this->pass_command = true;

    if (parameter.empty() || parameter != server.getPassword())
        return ;
    this->authentification = true;
}

void    Client::commandNick(Server &server, std::string const &parameter) 
{
    if (this->authentification == false)
    {
        try {
            if (this->pass_command == false)
            
                server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), parameter));
            else
                server.sendTemplate(*this, ERR_PASSWDMISMATCH(server.getName(), parameter));
            std::cout
                << "Client refused" << std::endl;
            server.clearClient(this);
            return;
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    else
    {
        try {
            if (parameter.empty())
            {
                server.sendTemplate(*this, ERR_NONICKNAMEGIVEN(server.getName(), "*"));
                server.clearClient(this);
                std::cout << "Client refused" << std::endl;
                return ;
            }
            for (size_t i = 0; i < server.getMaxClients(); i++)
            {
                if (server.getClients()[i] && server.getClients()[i]->getNickName() == parameter)
                {
                    server.sendTemplate(*this, ERR_NICKNAMEINUSE(server.getName(), parameter));
                    if(this->registered == false)
                    {
                        server.clearClient(this);
                        std::cout << "Client refused" << std::endl;
                    }
                    return ;
                }
            }
            if (parameter.length() > 9 || (!std::isalnum(parameter[0])))
            {
                if (this->nickName.empty())
                    this->nickName = "*";
                server.sendTemplate(*this, ERR_ERRONEUSNICKNAME(server.getName(), this->getNickName(), parameter));
                if(this->registered == false)
                {
                    server.clearClient(this);
                    std::cout << "Client refused" << std::endl;
                }
                return ;
            }
            for (size_t i = 1; i < parameter.size(); i++)
            {
                if (!std::isalnum(parameter[i]) && (parameter[i] != '-' || parameter[i] != '_' || parameter[i] != '|' ))
                {
                    if (this->nickName.empty())
                        this->nickName = "*";
                    server.sendTemplate(*this, ERR_ERRONEUSNICKNAME(server.getName(), this->getNickName(), parameter));
                    if(this->registered == false)
                    {
                        server.clearClient(this);
                        std::cout << "Client refused" << std::endl;
                    }
                    return ;
                }
            }
            if (!this->nickName.empty())
                server.replyNick(*this, parameter);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    this->setNickName(parameter);
    
}

void    Client::commandUser(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         userName;
    std::string         hostName;
    std::string         serverName;
    std::string         realName;

    if (this->nickName.empty())
    {
        try {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    try {
        datas >> userName;
        this->setUserName(userName);
        datas >> hostName;
        this->setHostName(hostName);
        datas >> serverName;
        this->setServerName(server.getName());
        realName = extractLast(parameter);
        if (realName.empty() || datas.fail())
        {
            server.sendTemplate(*this, ERR_NEEDMOREPARAMS(server.getName(), this->nickName, "USER"));
            server.clearClient(this);
            return ;
        }
        this->setRealName(realName);
        this->setSourceName();

        server.sendTemplate(*this, RPL_WELCOME(this->getServerName(), this->getNickName()));
        this->registered = true;

    }
    catch (std::exception &e) {
            throw;
    }
}

void    Client::commandQuit(Server &server, std::string const &parameter) {

    std::string reason = parameter;

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }

        reason = extractLast(parameter);
        if (reason.empty())
            reason = "leaving";
        server.replyQuit(*this, reason);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandPrivmsg(Server &server, std::string const &parameter)
{  
    std::istringstream  datas(parameter);
    std::string         recipient;
    std::string         message;

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
    }
    catch (std::exception &e) {
        throw ;
    }

    datas >> recipient;
    std::getline(datas >> std::ws, message);
    if (datas.fail())
    {
        try {
            server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "PRIVMSG"));
            return ;
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    message = extractLast(message);
    std::string channelName = recipient;
    if (channelName[0] != '#')
        channelName = "#" + channelName;
    if (server.getChannels().find(channelName) == server.getChannels().end())
    {
            if (!server.isClient(recipient))
            {
                try {
                    server.sendTemplate(*this, ERR_NOSUCHNICK(this->serverName, this->nickName, recipient));
                }
                catch (std::exception &e) {
                    throw ;
                }
                return ;
            }
            Client  *client = server.findClient(recipient);
            try {
                server.replyPrivmsgClient(*this, *client, message);
            }
            catch (std::exception &e) {
                throw ;
            }
    }
    else
    {
        recipient = channelName;
        std::map<std::string, Channel *>::iterator  channelIt = server.getChannels().find(recipient);
        if (channelIt == server.getChannels().end())
        {
            try {
                server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, recipient));
            }
            catch (std::exception &e) {
                throw ;
            }
            return ;
        }
        Channel *channel = channelIt->second;
        try {
            server.replyPrivmsgChannel(*this, *channel, message);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
}

void    Client::commandPart(Server &server, std::string const &parameter) {

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
    }
    catch (std::exception &e) {
        throw ;
    }

    std::string channelName = parameter;

    if (channelName[0] != '#')
        channelName = "#" + channelName;

    try {
        std::map<std::string, Channel *>::iterator  channelIt = server.getChannels().find(channelName);
        if (channelIt == server.getChannels().end())
        {
            server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
            return ;
        }
        Channel *channel = channelIt->second;

        server.replyPart(*this, channel);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandWho(Server &server, std::string const &parameter) 
{
    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
    }
    catch (std::exception &e) {
        throw ;
    }

    std::size_t found = parameter.find('#');
    if (found != std::string::npos)
    {
        Channel *channel = server.getChannels()[parameter];
        try {
            server.replyWho(*this, *channel);
        }
        catch (std::exception &e) {
            throw ;
        }
    }  
}

void    Client::commandPing(Server &server, std::string const &parameter) {

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
        server.replyPing(*this, parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandCap(Server &, std::string const &) {

    return;
}

void    Client::commandUnknown(Server &server, std::string const &parameter) {

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
        server.replyUnknown(*this, parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
}
