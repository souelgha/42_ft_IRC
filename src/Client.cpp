/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/13 17:39:23 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client(void) : authentification(false) {

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
    std::fill(mode, mode + 3, 0);
}

Client::~Client(void) { }

int Client::getFd() const {

    return(this->fd);
}

std::string Client::getIpAddress(void) const {

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

char    *Client::getMode(void) {

    return(this->mode);
}

void    Client::setFd(int fd) {

    this->fd = fd;
}

void    Client::setIpAddress(std::string address) {

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

    if (nickName.length() > 9)
        throw (std::runtime_error("Nickname too long\n"));
    this->nickName = nickName;
}

void    Client::setServerName(std::string const &serverName) {

    this->serverName = serverName;
}
void    Client::setSourceName(void) {

    std::string sourceName = this->nickName + "!" + this->userName + "@" + this->hostName;
    this->sourceName = sourceName;
}

void    Client::setMode(std::string const &mode) {
    
    for (size_t i = 0; mode[i]; i++)
    {
        if (mode[i] == '+')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                    this->mode[0] = 1;
                else if (mode[i] == 's')
                    this->mode[1] = 1;
                else if (mode[i] == 'w')
                    this->mode[2] = 1;
            }
        }
        if (mode[i] == '-')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                    this->mode[0] = 0;
                else if (mode[i] == 's')
                    this->mode[1] = 0;
                else if (mode[i] == 'w')
                    this->mode[2] = 0;
            }
        }
    }
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

void    Client::commandPass(Server &server, std::string const &parameter) {

    if (parameter.empty() || parameter != PASSWORD)
    {
        server.replyWrongPwd(*this);
    }
    this->authentification = true;
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
        std::cout << "<< " << message << std::endl;
        prefix = extractPrefix(message);
        command = extractCommand(message);
        parameter = message;
        this->handleCommand(server, prefix, command, parameter);
        buffer = this->buffer;
    }
}

void    Client::handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter) {
    
    void        (Client::*actions[11])(Server &, std::string const &) =
        {&Client::commandPass, &Client::commandNick, &Client::commandUser,
        &Client::commandMode, &Client::commandQuit,
        &Client::commandJoin, &Client::commandPart, &Client::commandPrivmsg,
        &Client::commandTopic,
        &Client::commandWhois, &Client::commandPing};
    std::string sent[] = {"PASS", "NICK", "USER", "MODE", "QUIT", "JOIN", "PART", "PRIVMSG", "TOPIC", "WHOIS", "PING"};
    int         i;

    for (i = 0; i < 11; i++)
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
    // throw ;
}

void    Client::commandNick(Server &server, std::string const &parameter) 
{
    for (size_t i = 0; i < server.getClients().size(); i++)
    {
        if (server.getClients()[i].getNickName() == parameter)
        {
            server.replyErrNick(*this);
            return;
        }
        if(parameter.length() > 9 || (!std::isalnum(parameter[0])))
        {
            server.replyErronNickUse(*this);
            return;
        }
        for (size_t i = 1; i < parameter.size(); i++)
        {
            if(!std::isalnum(parameter[i]) && (parameter[i] != '-' || parameter[i] != '_' || parameter[i] != '|' ))
            {
                server.replyErronNickUse(*this);
                return;
            }
        }            
    }
    server.replyNick(*this, parameter);
    this->setNickName(parameter);
    this->setSourceName();
    std::cout<< "Nvx NickName: <"<< this->nickName<< ">"<<std::endl;
    std::cout<< "sourceName: <"<< this->sourceName<< ">"<<std::endl;
}

std::string extractRealName(std::string parameter) {

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
        // if (datas.fail())
        //     throw UserCommandException();
        this->setUserName(userName);
        datas >> hostName;
        // if (datas.fail())
            // throw UserCommandException();
        this->setHostName(hostName);
        datas >> serverName;
        // if (datas.fail())
            // throw UserCommandException();
        this->setServerName(serverName);
        realName = extractRealName(parameter);
        // if (realName.empty())
            // throw UserCommandException();
        this->setRealName(realName);
        this->setSourceName();
     
   
        server.replyUser(*this);
        std::cout<<"welcome sourcename:<"<< sourceName<<">"<<std::endl;
    }
    catch (std::runtime_error &e) {
            throw;
    }
}




void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         recipient;
    std::string         mode;

    datas >> recipient;
    // verification a faire
    datas >> mode;
    try {
        if (server.getChannels().find(recipient) == server.getChannels().end())
        {
            char    previous[3];
            std::memcpy(previous, this->getMode(), 3);
            this->setMode(mode);
            size_t  i;
            for (i = 0; i < 3; i++)
            {
                if (previous[i] != this->getMode()[i])
                    break ;
            }
            if (i != 3)
                server.replyModeClient(*this);
            // std::cout << "Client:" <<std::endl
            //     << "fd: " << this->fd << std::endl
            //     << "adresse IP: " << this->ipAddress << std::endl
            //     << "real name: " << this->realName << std::endl
            //     << "host name: " << this->hostName << std::endl
            //     << "user name: " << this->userName << std::endl
            //     << "nickname: " << this->nickName << std::endl
            //     << "source kname: " << this->sourceName << std::endl
            //     << "server name: " << this->serverName << std::endl
            //     << "mode: " << this->mode << std::endl;
        }
        else
        {
            Channel &channel = server.getChannels()[recipient];
            channel.setMode(mode);
            server.replyModeChannel(*this, channel);
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

void    Client::commandWhois(Server &server, std::string const &) {

    // verification a faire parametre et client
    try {
        server.replyWhois(*this);
    }
    catch (std::exception &e) {
        std::cout << "Error in Whois" << std::endl;
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

void    Client::commandJoin(Server &server, std::string const &parameter)
{  
    std::string channelName = "";

    if (parameter [0] != '#' && parameter[0] != '$')    // verifier les symboles # et $ d'appelation de canaux.
        channelName = "#";
    channelName += parameter;
    Channel channel = server.findChannel(*this, channelName);
    // Channel channel = server.findChannel(*this, channelName);
    // if(!channel.isOperator(this->nickName))
    //     channel.AddUser(*this);  
    
    server.replyJoin(*this, channel);
}

void    Client::commandPart(Server &server, std::string const &parameter) {

    std::string channelName = "";

    if (parameter [0] != '#' && parameter[0] != '$')    // verifier les symboles # et $ d'appelation de canaux.
        channelName = "#";
    channelName += parameter;
    Channel &channel = server.getChannels()[channelName];
    server.replyPart(*this, channel);
}

void    Client::commandPrivmsg(Server &server, std::string const &parameter)
{  
    std::istringstream  datas(parameter);
    std::string         recipient;
    std::string         message;

    datas >> recipient;
    std::getline(datas >> std::ws, message);
    message = message.substr(1, message.length() - 1);
    // if (datas.fail())
    //     throw(std::runtime_error("Not enough parameters\n"));
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
        server.replyPrivmsgChannel(*this, channel, message);
    }
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


/*
server.reply(*this, ERR_ERRONEUSNICKNAME(this->serverName, this->nickName));
server.reply(*this, ERR_NEEDMOREPARMs(this->serverName, this->nickName, "USER") );
*/