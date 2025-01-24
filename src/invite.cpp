#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void    Client::commandInvite(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         nickname;
    std::string         channelName;
    Client              *recipient;

    datas >> nickname;
    datas >> channelName;
    if (datas.fail())
    {
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "INVITE"));
        return ;
    }
    if (!server.isClient(nickname))
    {
        server.sendTemplate(*this, ERR_NOSUCHNICK(this->serverName, this->nickName, nickname));
        return ;
    }
    std::map<std::string, Channel *>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel *channel = channelIt->second;
    if (!channel->isUser(this->nickName))
    {
        server.sendTemplate(*this, ERR_NOTONCHANNEL(this->serverName, this->nickName, channel->getName()));
        return ;
    }
    if (!channel->isOperator(this->nickName))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel->getName()));
        return ;
    }
    if (channel->isUser(nickname))
    {
        server.sendTemplate(*this, ERR_USERONCHANNEL(this->serverName, nickName, channel->getName()));
        return ;
    }
    for (size_t i = 0; i < server.getMaxClients(); i++)
    {
        if (server.getClients()[i] && server.getClients()[i]->getNickName() == nickname)
            recipient = server.getClients()[i];
    }
    server.replyInvite(*this, *recipient, *channel);
}

void    Server::replyInvite(Client const &sender, Client const &recipient, Channel &channel) {

    std::string message = RPL_INVITING(sender.getServerName(), sender.getNickName(), recipient.getNickName(), channel.getName());

    sendTemplate(sender, message);
    sendTemplate(recipient, message);
    channel.addInvited(recipient.getNickName());
}