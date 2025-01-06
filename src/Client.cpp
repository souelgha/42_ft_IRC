/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/06 15:43:04 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) { }


Client::~Client(void) { }

int Client::getFd() const {
    return(this->cliFd);
}

std::string Client::getIpAddr() const {
    return(this->cliAddr);
}

std::string const   &Client::getRealName() const {
    return(this->realName);
}

std::string const   &Client::getHostName() const {
    return(this->hostName);
}

std::string const   &Client::getUserName() const {
    return(this->userName);
}

std::string const   &Client::getNickName() const {
    return(this->nickName);
}

std::string const   &Client::getServerName() const {
    return(this->serverName);
}

void    Client::setFd(int fd) {

    this->cliFd = fd;
}
void    Client::setIpAdd(std::string addr) {
    this->cliAddr = addr;
}

void    Client::setRealName(std::string const &realName) {

    this->userName = realName;
}

void    Client::setHostName(std::string const &hostName) {

    this->hostName = hostName;
}

void    Client::setUserName(std::string const &userName) {

    this->userName = userName;
}

void    Client::setNickName(std::string const &nickName) {

    if (nickName.length() > 9)
        this->nickName = nickName;
}

void    Client::setServerName(std::string const &serverName) {

    this->serverName = serverName;
}
