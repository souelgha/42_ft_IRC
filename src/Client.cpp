/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/22 12:19:22 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : authentification(false) {

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
}

Client::~Client(void) { }

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

std::string Client::extractMessage(std::string const &buffer) {

    std::string message;
    size_t      end;
    std::string remainder;

    end = buffer.find(DELIMITER);
    message = buffer.substr(0, end);
    remainder = buffer.substr(end + 2, buffer.length() - (end + 2));
    std::fill(this->buffer, this->buffer + BUFFER_SIZE, 0);
    std::memcpy(this->buffer, remainder.c_str(), remainder.length());
    return (message);
}

std::string Client::extractPrefix(std::string &message) {

    std::istringstream  split(message);
    std::string         prefix = "";

    if (message[0] == ':')
        split >> prefix;
    std::getline(split >> std::ws, message);
    return (prefix);
}

std::string Client::extractCommand(std::string &message) {

    std::istringstream  split(message);
    std::string         command;

    split >> command;
    std::getline(split >> std::ws, message);
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
            << CYAN << "<< " << message << WHITE << std::endl;

        prefix = this->extractPrefix(message);
        command = this->extractCommand(message);
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
    
  
    void        (Client::*actions[12])(Server &, std::string const &) =
        {&Client::commandNick, &Client::commandUser,
        &Client::commandMode, &Client::commandQuit, // retravailler la commande QUIT
        &Client::commandJoin, &Client::commandPart, &Client::commandPrivmsg,
        &Client::commandKick, &Client::commandInvite, &Client::commandTopic,
        &Client::commandWho, &Client::commandPing};
    std::string sent[] = {"NICK", "USER", "MODE", "QUIT", "JOIN", "PART", "PRIVMSG", "KICK", "INVITE", "TOPIC", "WHO", "PING"};
    int         i;

    for (i = 0; i < 12; i++)
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

// void    Client::commandPass(Server &server, std::string const &parameter) {

//     if (parameter.empty() || parameter != server.getPassword())
//     {
//         try {
//             server.replyWrongPwd(*this);
//         }
//         catch (std::exception &e) {
//             throw ;
//         }
//     }
//     this->authentification = true;
// }

void    Client::commandNick(Server &server, std::string const &parameter) 
{
    for (size_t i = 0; i < server.getClients().size(); i++)
    {
        if (server.getClients()[i].getNickName() == parameter)
        {
            try {
                server.replyErrNick(*this);
            }
            catch (std::exception &e) {
                throw ;
            }
            return;
        }
        if(parameter.length() > 9 || (!std::isalnum(parameter[0])))
        {
            try {
                server.replyErronNickUse(*this);
            }
            catch (std::exception &e) {
                throw ;
            }
            return;
        }
        for (size_t i = 1; i < parameter.size(); i++)
        {
            if(!std::isalnum(parameter[i]) && (parameter[i] != '-' || parameter[i] != '_' || parameter[i] != '|' ))
            {
                try {
                    server.replyErronNickUse(*this);
                }
                catch (std::exception &e) {
                    throw ;
                }
                return;
            }
        }            
    }
    try {
        server.replyNick(*this, parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
    this->setNickName(parameter);
    this->setSourceName();
}

static std::string extractRealName(std::string parameter) {

    if (parameter.empty())
        return ("");

    size_t i = parameter.find(':');
    if (i == std::string::npos)
        throw std::runtime_error("Wrong format in USER command\n");

    std::string realName = parameter.substr(i + 1, parameter.length() - i);
    return (realName);
}

void    Client::commandUser(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         userName;
    std::string         hostName;
    std::string         serverName;
    std::string         realName;

    try {
        datas >> userName;
        this->setUserName(userName);
        datas >> hostName;
        this->setHostName(hostName);
        datas >> serverName;
        this->setServerName(serverName);
        realName = extractRealName(parameter);
        this->setRealName(realName);
        this->setSourceName();

        try {
            server.replyUser(*this);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    catch (std::exception &e) {
            throw;
    }
}

void    Client::commandQuit(Server &server, std::string const &parameter) {

    try {
        std::string   reason = parameter;
        if (!parameter.empty())
            reason = parameter.substr(1, parameter.length() - 1);
        try {
            server.replyQuit(*this, reason);
        }
        catch (std::exception &e) {
            throw ;
        }
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

    datas >> recipient;
    std::getline(datas >> std::ws, message);
    if (datas.fail())
    {
        try {
            server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "PRIVMSG"));
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    message = message.substr(1, message.length() - 1);
    if (server.getChannels().find(recipient) == server.getChannels().end())
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
            Client  client = server.findClient(recipient);
            try {
                server.replyPrivmsgClient(*this, client, message);
            }
            catch (std::exception &e) {
                throw ;
            }
    }
    else
    {
        std::map<std::string, Channel>::iterator    channelIt = server.getChannels().find(recipient);
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
        Channel &channel = channelIt->second;
        try {
            server.replyPrivmsgChannel(*this, channel, message);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
}

void    Client::commandWho(Server &server, std::string const &parameter) 
{
    std::size_t found = parameter.find('#');
    if (found != std::string::npos)
    {
        Channel &channel = server.getChannels()[parameter];
        try {
            server.replyWho(*this, channel);
            }
        catch (std::exception &e) {
            throw ;
        }
    }  
}

void    Client::commandPing(Server &server, std::string const &parameter) {

    try {
        server.replyPing(*this, parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandUnknown(Server &server, std::string const &parameter) {

    try {
        server.replyUnknown(*this, parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandPart(Server &server, std::string const &parameter) {

    std::string      channelName = "";

    if (parameter [0] != '#')
        channelName = "#";
    channelName += parameter;

    std::map<std::string, Channel>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel &channel = channelIt->second;

    try {
        server.replyPart(*this, channel);
    }
    catch (std::exception &e) {
        throw ;
    }
}

//done
void    Client::commandTopic(Server &server, std::string const &parameter) {

    std::cout<<"parameter : <" <<parameter<<">" << std::endl;
    if(parameter == "TOPIC ")
    {   
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "TOPIC"));
        return;
    }   

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         newTopic;
    
    datas >> channelName;    
    std::getline(datas >> std::ws, newTopic);
    newTopic = newTopic.substr(1, newTopic.length() - 1);

    std::map<std::string, Channel>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel &channel = channelIt->second;
    if(channel.getTMode() && (!channel.isOperator(this->nickName)))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel.getName()));
        return;
    }
    server.replyTopic(*this, channel, newTopic);
}
