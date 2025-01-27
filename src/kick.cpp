
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void    Client::commandKick(Server &server, std::string const &parameter) {

    try {
        if (this->registered == false)
        {
            server.sendTemplate(*this, ERR_NOTREGISTERED(server.getName(), "*"));
            server.clearClient(this);
            return;
        }
    }
    catch (std::exception &e) {
        throw ;
    }

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         nickname;
    std::string         reason;
    Client              *recipient;

    datas >> channelName;
    datas >> nickname;
    if (datas.fail())
    {
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "KICK"));
        return ;
    }
    if (channelName[0] != '#')
        channelName = "#" + channelName;
    std::getline(datas >> std::ws, reason);
    reason = this->extractLast(reason);
    if (reason.empty())
        reason = this->nickName;

    std::map<std::string, Channel *>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel *channel = channelIt->second;

    if (!channel->isOperator(this->nickName))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel->getName()));
        return ;
    }

    if (!channel->isUser(nickname))
    {
        server.sendTemplate(*this, ERR_USERNOTINCHANNEL(this->serverName, this->nickName, nickName, channel->getName()));
        return ;
    }

    if (!channel->isUser(this->nickName))
    {
        server.sendTemplate(*this, ERR_NOTONCHANNEL(this->serverName, this->nickName, channel->getName()));
        return ;
    }

    for (size_t i = 0; i < channel->getUsers().size(); i++)
    {
        if (channel->getUsers()[i]->getNickName() == nickname)
            recipient = channel->getUsers()[i];
    }
    
    server.replyKick(*this, channel, recipient, reason);
}
