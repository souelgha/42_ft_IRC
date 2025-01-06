/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/06 14:32:20 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Client.hpp"
#include "Server.hpp"

#define DELIMITER "\r\n"

void commandNick(std::string const &message, Client client) {

    std::string nickname;

    nickname = message.substr(5, message.length() - 4);
    std::cout << "NickName: <" << nickname << ">" << std::endl;
    try {
        client.setNickName(nickname);
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::string getRealName(std::string message) {

    size_t i = message.find(':');

    // if (i == std::string::npos)
    //     throw Error;
    return (message.substr(i + 1, message.length() - i));
}

void commandUser(std::string const &message, Client client) {

    std::string         userName;
    std::string         hostName;
    std::string         serverName;
    std::string         realName;
    std::istringstream  datas(message);

    std::string command;
    datas >> command;
    try {
        datas >> userName;
        client.setUserName(userName);
        datas >> hostName;
        client.setHostName(hostName);
        datas >> serverName;
        client.setServerName(serverName);
        realName = getRealName(message);
        std::cout << "Real name: <" << realName << ">" << std::endl;
        client.setRealName(realName);
    }
    catch (std::exception &e) {
        throw;
    }
}

std::string getCommand(std::string message) {

    size_t i = message.find(' ');

    if (i == std::string::npos)
        return (message);
    return (message.substr(0, i));
}

void    sendAnswer(Client const &client) {

    const char msg[] = ":127.0.0.1 001 stouitou :Welcome to the IRC Network stouitou!stouitou@stouitou\r\n";
    int sentBytes = send(client.getFd(), msg, strlen(msg), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
}

int main()
{
  Server serv;
  
  try
  {
    signal(SIGINT, Server::SignalCatch);
    serv.ServerInit();
  }
  catch(const std::exception& e)
  {
    serv.CloseFds();
    std::cerr << e.what() << "ici"<< '\n';    
  }
    return(0);
}