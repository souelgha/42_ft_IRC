/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/09 10:07:17 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
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

std::string const   &Client::getMode(void) const {

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

std::string extractPrefix(std::string &message) {

    std::istringstream  split(message);
    std::string         prefix = "";

    if (message[0] == ':')
        split >> prefix;
    std::getline(split >> std::ws, message);
    return (prefix);
}

std::string extractCommand(std::string &message) {

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
        // std::cout << "Message: <" << message << ">" << std::endl;
        prefix = extractPrefix(message);
        // std::cout << "Prefix: <" << prefix << ">" << std::endl;
        command = extractCommand(message);
        // std::cout << "Command: <" << command << ">" << std::endl;
        parameter = message;
        // std::cout << "Parameter: <" << parameter << ">" << std::endl;
        this->handleCommand(server, prefix, command, parameter);
        buffer = this->buffer;
    }
}

void    Client::commandNick(Server &server, std::string const &parameter) {

    std::string nickname;

    try {
        for (size_t i = 0; i < server.getClients().size(); i++)
        {
            if (server.getClients()[i].getNickName() == parameter)
                throw (std::runtime_error("Nickname not available\n"));
        }
        this->setNickName(parameter);
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::string extractRealName(std::string parameter) {

    size_t i = parameter.find(':');

    // if (i == std::string::npos)
    //     throw Error;
    return (parameter.substr(i + 1, parameter.length() - i));
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
        std::string message = ":"
            + this->serverName + " 001 "
            + this->nickName + " :Welcome to the IRC Network "
            + this->nickName + "!"
            + this->nickName + "@"
            + this->nickName + "\r\n";
        server.reply(*this, message);
    }
    catch (std::exception &e) {
        throw;
    }
}

void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         mode;
    std::string         nickname;

    datas >> nickname;
    // verification a faire
    datas >> mode;
    try {
        this->setMode(mode);
        std::string message = ":"
            + this->serverName + " MODE "
            + this->nickName + " :"
            + this->mode + "\r\n";
        server.reply(*this, message);
        // std::cout << "Client:" <<std::endl
        //     << "fd: " << this->fd << std::endl
        //     << "adresse IP: " << this->ipAddress << std::endl
        //     << "real name: " << this->realName << std::endl
        //     << "host name: " << this->hostName << std::endl
        //     << "user name: " << this->userName << std::endl
        //     << "nickname: " << this->nickName << std::endl
        //     << "server name: " << this->serverName << std::endl
        //     << "mode: " << this->mode << std::endl;
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::commandWhois(Server &server, std::string const &) {

    // verification a faire parametre et client
    try {
        std::string message = ":"
            + this->serverName + " 311 "
            + this->nickName + " "
            + this->userName + " "
            + this->hostName + " * :"
            + this->realName + "\r\n";
        server.reply(*this, message);
    }
    catch (std::exception &e) {
        std::cout << "Error in Whois" << std::endl;
        throw ;
    }
}

void    Client::commandPing(Server &server, std::string const &parameter) {

    try {
        std::string message = ":"
            + this->serverName + " PONG :"
            + parameter + "\r\n";
        server.reply(*this, message);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Client::handleCommand(Server &server, std::string const &, std::string const &command, std::string const &parameter) {
    
    void        (Client::*actions[5])(Server &, std::string const &) =
        {&Client::commandNick, &Client::commandUser, &Client::commandMode,
        &Client::commandWhois, &Client::commandPing};
    std::string sent[] = {"NICK", "USER", "MODE", "WHOIS", "PING"};
    int         i;

    for (i = 0; i < 5; i++)
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
