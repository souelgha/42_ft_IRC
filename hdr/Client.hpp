/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:27 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/07 15:30:45 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>

class   Client
{
    private:

        int             fd;         // fd du client
        std::string     ipAddress;  // addresse socket client

        std::string     realName;
        std::string     hostName;
        std::string     userName;
        std::string     nickName;
        std::string     serverName;
        std::string     mode;

    public:

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
        std::string const   &getNickName() const;
        std::string const   &getServerName() const;
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
};
