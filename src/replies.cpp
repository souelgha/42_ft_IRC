#include"Server.hpp"
#include"Client.hpp"

void    Server::replyMissPara(Client &client, std::string &command) {

    std::string const message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickName(), command);
    sendTemplate(client, message);
}

void    Server::sendTemplate(Client const &client, std::string const &message){
    
    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
        throw(std::runtime_error("Failed to send message to client\n")) ;
}

void    Server::replyNick(Client &client, std::string const &newnick) {
    std::string const message = ":"+client.getSourceName()+  " NICK :"+newnick+ CRLF;
    sendTemplate(client, message);
}

void    Server::replyErrNick(Client &client) {
    
    std::string const message = ERR_NICKNAMEINUSE(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}

void    Server::replyErronNickUse(Client &client) {
    
    std::string const message = ERR_ERRONEUSNICKNAME(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}

void    Server::replyWho(Client const &client, Channel &channel) 
{
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        std::string message = RPL_WHOREPLY(client.getServerName(), client.getNickName(), 
                                channel.getName(), channel.getUsers()[i].getNickName(), 
                                channel.getUsers()[i].getUserName(), channel.getUsers()[i].getHostName(), 
                                channel.getUsers()[i].getRealName());
        sendTemplate(client, message);
    }
    std::string message = RPL_ENDOFWHO(client.getServerName(), client.getNickName(), channel.getName());
    sendTemplate(client, message);

}

void    Server::replyPrivmsgClient(Client const &sender, Client const &recipient, std::string const &toSend) {

    std::string message = ":" + sender.getSourceName() + " PRIVMSG " + recipient.getNickName() + " :" + toSend + "\r\n";

    sendTemplate(recipient, message);
}

void    Server::replyPrivmsgChannel(Client const &sender, Channel &channel, std::string const &toSend) {

    std::string message = ":" + sender.getSourceName() + " PRIVMSG " + channel.getName() + " :" + toSend + "\r\n";

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        if (channel.getUsers()[i].getNickName() != sender.getNickName())
            sendTemplate(channel.getUsers()[i], message);
    }
}

void    Server::replyTopic(Client const &client, Channel &channel, std::string const &topic) {

    std::string message = RPL_TOPIC(client.getSourceName(), channel.getName(), topic);

    for (size_t i = 0; i < channel.getUsers().size(); i++)
        sendTemplate(channel.getUsers()[i], message);
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

void    Server::replyKick(Client const &client, Channel &channel, Client const &recipient, std::string const &reason) {

    std::string message = RPL_KICK(client.getSourceName(), channel.getName(), recipient.getNickName(), reason);

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        sendTemplate(channel.getUsers()[i], message);
    }
    channel.remUser(recipient);
    channel.remOper(recipient.getNickName());
    channel.remInvited(recipient.getNickName());
}

void    Server::replyPart(Client const &client, Channel &channel) {

    std::string message = ":" + client.getSourceName() + " PART " + channel.getName() + "\r\n";

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
        sendTemplate(channel.getUsers()[i], message);
    channel.remUser(client);
    channel.remOper(client.getNickName());
    channel.remInvited(client.getNickName());
    if (channel.getUsers().empty())
        this->deleteChannel(channel.getName());
}

void    Server::replyQuit(Client &client, std::string const &reason) {

    std::string message = ":"
            + client.getServerName()
            + " QUIT";

    if (!reason.empty())
        message += ": " + reason;
    message += CRLF;

    sendTemplate(client, message);
    clearClient(client.getFd());
}

