/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:26:33 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/24 17:08:09 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void    Server::sendTemplate(Client const &client, std::string const &message){
    
    std::cout << ">> " << message << std::flush;
    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
        throw (std::runtime_error("Failed to send message to client")) ;
}

void    Server::replyNick(Client &client, std::string const &newnick) {

    std::string const message = ":" + client.getSourceName() + " NICK :"+ newnick + CRLF;
    sendTemplate(client, message);
}

void    Server::replyQuit(Client &client, std::string const &reason) {

    std::string message = ":" + client.getServerName() + " QUIT: " + reason + CRLF;

    try {
        sendTemplate(client, message);
        clearClient(&client);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyPrivmsgClient(Client const &sender, Client const &recipient, std::string const &toSend) {

    std::string message = ":" + sender.getSourceName() + " PRIVMSG " + recipient.getNickName() + " :" + toSend + "\r\n";

    try {
        sendTemplate(recipient, message);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyPrivmsgChannel(Client const &sender, Channel &channel, std::string const &toSend) {

    std::string message = ":" + sender.getSourceName() + " PRIVMSG " + channel.getName() + " :" + toSend + "\r\n";

    try {
        for (size_t i = 0; i < channel.getUsers().size(); i++)
        {
            if (channel.getUsers()[i]->getNickName() != sender.getNickName())
                sendTemplate(*channel.getUsers()[i], message);
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyPart(Client const &client, Channel *channel) {

    std::string message = ":" + client.getSourceName() + " PART " + channel->getName() + DELIMITER;

    try {
        for (size_t i = 0; i < channel->getUsers().size(); i++)
            sendTemplate(*channel->getUsers()[i], message);
        channel->remUser(client);
        if (channel->getUsers().empty())
            this->deleteChannel(channel);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyKick(Client const &client, Channel *channel, Client *recipient, std::string const &reason) {

    if (channel == NULL || recipient == NULL)
        return ;

    std::string message = RPL_KICK(client.getSourceName(), channel->getName(), recipient->getNickName(), reason);

    if (channel == NULL || recipient == NULL)
        return ;
    try {
        for (size_t i = 0; i < channel->getUsers().size(); i++)
            sendTemplate(*channel->getUsers()[i], message);
        channel->remUser(*recipient);
        for (size_t i = 0; i < recipient->getListChannels().size(); i++)
        {
            if (recipient->getListChannels()[i]->getName() == channel->getName())
                recipient->getListChannels().erase(recipient->getListChannels().begin() + i);
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyTopic(Client const &client, Channel &channel, std::string const &topic) {

    std::string message = RPL_TOPIC(client.getSourceName(), channel.getName(), topic);

    try {
        for (size_t i = 0; i < channel.getUsers().size(); i++)
            sendTemplate(*channel.getUsers()[i], message);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::replyWho(Client const &client, Channel &channel) 
{
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        std::string message = RPL_WHOREPLY(client.getServerName(), client.getNickName(), 
                                channel.getName(), channel.getUsers()[i]->getNickName(), 
                                channel.getUsers()[i]->getUserName(), channel.getUsers()[i]->getHostName(), 
                                channel.getUsers()[i]->getRealName());
        sendTemplate(client, message);
    }
    std::string message = RPL_ENDOFWHO(client.getServerName(), client.getNickName(), channel.getName());
    sendTemplate(client, message);
}

void    Server::replyPing(Client const &client, std::string const &pong) {

    std::string message = ":"
            + client.getServerName() + " PONG :" 
            + pong + CRLF;
    sendTemplate(client, message);
}

void    Server::replyUnknown(Client const &client, std::string const &command) {

    std::string message = "Unknown command " + command + CRLF;
    sendTemplate(client, message);
}
