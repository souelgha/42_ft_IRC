/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:30 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/07 15:59:34 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) { }


Client::~Client(void) { }

int Client::getFd() const {

    return(this->fd);
}

std::string Client::getIpAddress() const {

    return(this->ipAddress);
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

std::string Client::getNickName() const {

    return(this->nickName);
}

std::string const   &Client::getServerName() const {

    return(this->serverName);
}

std::string const   &Client::getMode() const {

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

    // if (nickName.length() > 9)
    //     throw ;
    this->nickName = nickName;
}

void    Client::setServerName(std::string const &serverName) {

    this->serverName = serverName;
}

void    Client::setMode(std::string const &mode) {

    this->mode = mode;
}
