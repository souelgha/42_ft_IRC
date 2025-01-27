#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


void    Client::commandTopic(Server &server, std::string const &parameter) {

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

    if (parameter.empty())
    {   
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "TOPIC"));
        return;
    }   

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         newTopic;
    
    datas >> channelName;
    if (channelName[0] != '#')
        channelName = "#" + channelName;
    std::map<std::string, Channel *>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel *channel = channelIt->second;

    std::getline(datas >> std::ws, newTopic);
    if (!datas.fail())
        newTopic = this->extractLast(newTopic);

    if (channel->getTMode() && (!channel->isOperator(this->nickName)))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel->getName()));
        return;
    }
    server.replyTopic(*this, *channel, newTopic);
}
