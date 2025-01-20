
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


void    Client::commandKick(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         nickname;
    std::string         reason;
    Client              recipient;

    datas >> channelName;
    datas >> nickname;
    if (datas.fail())
    {
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "KICK"));
        return ;
    }
    std::getline(datas >> std::ws, reason);
    reason = reason.substr(1, reason.length() - 1);

    std::map<std::string, Channel>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel &channel = channelIt->second;

    if (!channel.isOperator(this->nickName))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel.getName()));
        return ;
    }

    if (!channel.isUser(nickname))
    {
        server.sendTemplate(*this, ERR_USERNOTINCHANNEL(this->serverName, this->nickName, nickName, channel.getName()));
        return ;
    }

    if (!channel.isUser(this->nickName))
    {
        server.sendTemplate(*this, ERR_NOTONCHANNEL(this->serverName, this->nickName, channel.getName()));
        return ;
    }

    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        if (channel.getUsers()[i].getNickName() == nickname)
            recipient = channel.getUsers()[i];
    }
    
    if (reason.empty())
        reason = this->nickName;
    server.replyKick(*this, channel, recipient, reason);
}
//done
