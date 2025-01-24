#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


static std::string const    extractLast(std::string const &parameter) {

    size_t  i = parameter.find(':');
    if (i == std::string::npos)
        return (parameter);

    std::string reason = parameter.substr(i + 1, parameter.length() - i);
    return (reason);
}
void    Client::commandTopic(Server &server, std::string const &parameter) {

    if (parameter.empty())
    {   
        server.sendTemplate(*this, ERR_NEEDMOREPARAMS(this->serverName, this->nickName, "TOPIC"));
        return;
    }   

    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         newTopic;
    
    datas >> channelName;
    std::map<std::string, Channel *>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel *channel = channelIt->second;

    std::getline(datas >> std::ws, newTopic);
    if (!datas.fail())
        newTopic = extractLast(newTopic);

    if (channel->getTMode() && (!channel->isOperator(this->nickName)))
    {
        server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel->getName()));
        return;
    }
    server.replyTopic(*this, *channel, newTopic);
}

void    Client::commandCap(Server &, std::string const &) {

    return;
}
