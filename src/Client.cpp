/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/14 17:25:32 by stouitou         ###   ########.fr       */
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

std::string const   &Client::getNickName(void) const {

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

static std::string extractPrefix(std::string &message) {

    std::istringstream  split(message);
    std::string         prefix = "";

    if (message[0] == ':')
        split >> prefix;
    std::getline(split >> std::ws, message);
    return (prefix);
}

static std::string extractCommand(std::string &message) {

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
        message = extractMessage(buffer);
        std::cout << YELLOW << "<< " << message << WHITE << std::endl;
        prefix = extractPrefix(message);
        command = extractCommand(message);
        parameter = message;
        this->handleCommand(server, prefix, command, parameter);
        buffer = this->buffer;
    }
}

void    Client::handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter) {
    
    void        (Client::*actions[13])(Server &, std::string const &) =
        {&Client::commandPass, &Client::commandNick, &Client::commandUser,
        &Client::commandMode, &Client::commandQuit, // retravailler la commande QUIT
        &Client::commandJoin, &Client::commandPart, &Client::commandPrivmsg,
        &Client::commandKick, &Client::commandInvite, &Client::commandTopic,
        &Client::commandWho, &Client::commandPing};
    std::string sent[] = {"PASS", "NICK", "USER", "MODE", "QUIT", "JOIN", "PART", "PRIVMSG", "KICK", "INVITE", "TOPIC", "WHO", "PING"};
    int         i;

    for (i = 0; i < 13; i++)
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

bool    Client::commandConnect(Server &server) 
{
    std::string buffer = this->buffer;
    // std::cout << RED<< "buf connect " << buffer<< WHITE << std::endl;
    std::size_t cap=buffer.find("CAP");
    std::size_t pwd = buffer.find("PASS");
    std::size_t data = buffer.find(PASSWORD);
    if(cap!=std::string::npos && pwd==std::string::npos)
    {
        server.replyWrongConnect(*this);
        return(false);
    }
    else if(cap!=std::string::npos && pwd!=std::string::npos && data==std::string::npos)
    {
        server.replyWrongConnect(*this);
        return(false);
    }
    else
        return(true);
}

void    Client::commandPass(Server &server, std::string const &parameter) {

    if (parameter != PASSWORD)
    {
        server.replyWrongConnect(*this);
    }
    this->authentification = true;
}
//si trouve CAP LS => check si PASS existe

void    Client::commandNick(Server &server, std::string const &parameter) 
{
    std::string oldnick;
//check si nick name is used ERR_NICKNAMEINUSE
//char invalids ERR_ERRONEUSNICKNAME// manque le parametre de du nickname ERR_NONICKNAMEGIVEN
//message a envoye quand on change de Nickmane: "commande =>:WiZ NICK Kilroy          ; message => WiZ changed his nickname to Kilroy.
//:Guest65529!~sonouelg@c88c-7273-6bb-ab79-b772.210.62.ip NICK :fred74477
    
        for (size_t i = 0; i < server.getClients().size(); i++)
        {
            if (server.getClients()[i].getNickName() == parameter)
            {
                // server.replyErrNick(*this);
                return;
            }
        }
        oldnick = this->nickName;
        // server.replyNick(*this, parameter);
        // :Guest65529!~sonouelg@c88c-7273-6bb-ab79-b772.210.62.ip NICK :fred74477   
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

    // if (this->authentification == false)
    //     throw (std::runtime_error("Not authentified\n"));
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

        server.replyUser(*this);
    }
    catch (std::exception &e) {
            throw;
    }
}

static std::string  adjustMode(std::string &parameter) {

    std::string mode = "";
    size_t      i = 0;

    if (parameter[i] != '+' && parameter[i] != '-')
        return (mode);
    mode += parameter[i];
    i++;
    for (; parameter[i] && (parameter[i] == 'i' || parameter[i] == 'k' || parameter[i] == 'l' || parameter[i] == 'o' || parameter[i] == 't'); i++)
        mode += parameter[i];
    return (mode);
}

void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         recipient;
    std::string         value;
    std::string         mode;

    datas >> recipient;
    datas >> value;
    mode = adjustMode(value);
    try {
        if (server.getChannels().find(recipient) == server.getChannels().end())
        {
            this->setMode(mode);
            server.replyModeClient(*this);
        }
        else
        {
            Channel &channel = server.getChannels()[recipient];
            if (channel.isOperator(this->nickName))
                server.replyModeChannel(*this, channel, mode);
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandQuit(Server &server, std::string const &parameter) {

    try {
        std::string   reason = parameter;
        if (!parameter.empty())
            reason = parameter.substr(1, parameter.length() - 1);
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

    datas >> recipient;
    std::getline(datas >> std::ws, message);
    message = message.substr(1, message.length() - 1);
    if (server.getChannels().find(recipient) == server.getChannels().end())
    {
        try {
            Client  client = server.findClient(recipient);
            server.replyPrivmsgClient(*this, client, message);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    else
    {
        Channel &channel = server.getChannels()[recipient];
        try {
            server.replyPrivmsgChannel(*this, channel, message);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
}

void    Client::commandWho(Server &server, std::string const &) {

    try {
        server.replyWho(*this);
    }
    catch (std::exception &e) {
        throw ;
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

void    Client::commandJoin(Server &server, std::string const &parameter)
{  
    std::string channelName = "";

    if (parameter [0] != '#')
        channelName = "#";
    channelName += parameter;
    Channel channel = server.findChannel(*this, channelName);
    
    try {
        server.replyJoin(*this, channel);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandPart(Server &server, std::string const &parameter) {

    std::string channelName = "";

    if (parameter [0] != '#')
        channelName = "#";
    channelName += parameter;
    Channel &channel = server.getChannels()[channelName];

    try {
        server.replyPart(*this, channel);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandKick(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         nickname;
    std::string         reason;
    Client              recipient;

    // if (!channel.isOperator(this->getnickname())
        // return error ;
    datas >> channelName;
    datas >> nickname;
    std::getline(datas >> std::ws, reason);
    reason = reason.substr(1, reason.length() - 1);

    Channel &channel = server.getChannels()[channelName];
    // proteger si channel n'existe pas

    if (!channel.IsUser(nickname))
        return ;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        if (channel.getUsers()[i].getNickName() == nickname)
            recipient = channel.getUsers()[i];
    }
    
    if (reason.empty())
        reason = this->nickName;
    server.replyKick(*this, channel, recipient, reason);
}

void    Client::commandInvite(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         nickname;
    std::string         channelName;
    Client              recipient;

    datas >> nickname;
    datas >> channelName;

    Channel &channel = server.getChannels()[channelName];
    // proteger si channel n'existe pas

    for (size_t i = 0; i < server.getClients().size(); i++)
    {
        if (server.getClients()[i].getNickName() == nickname)
            recipient = server.getClients()[i];
    }
    server.replyInvite(*this, recipient, channel);
}

void    Client::commandTopic(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         newTopic;

    // if (!channel.isOperator(this->getNickName())
        // return error ;
    datas >> channelName;
    std::getline(datas >> std::ws, newTopic);
    newTopic = newTopic.substr(1, newTopic.length() - 1);

    Channel &channel = server.getChannels()[channelName];
    // proteger si channel n'existe pas

    server.replyTopic(*this, channel, newTopic);
}
