/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:27 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/06 13:13:37 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>

class   Client
{
    private:

        int         cliFd;      // fd du client
        std::string cliAddr;    // addresse socket client

        std::string realName;
        std::string hostName;
        std::string userName;
        std::string nickName;
        std::string serverName;

    public:

        /* CONSTRUCTORS */
        Client(void);

        /* DESTRUCTORS */
        ~Client(void);

        /* GETTERS */
        int                 getFd() const;
        std::string         getIpAddr() const;
        std::string const   &getRealName() const;
        std::string const   &getHostName() const;
        std::string const   &getUserName() const;
        std::string          getNickName() const; // mod sonia
        std::string const   &getServerName() const;

        /* SETTERS */
        void                setFd(int fd);
        void                setIpAdd(std::string addr);
        void                setRealName(std::string const &realName);
        void                setHostName(std::string const &hostName);
        void                setUserName(std::string const &userName);
        void                setNickName(std::string const &nickName);
        void                setServerName(std::string const &serverName);
};
