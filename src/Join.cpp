#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


void    Client::commandJoin(Server &server, std::string const &parameter)
{  
    // std::string channelName = "";
    std::istringstream  datas(parameter);
    std::string         channelName;
    std::string         keyvalue;

    datas >> channelName;
    datas >> keyvalue;

    // if (parameter [0] != '#')
    //     channelName = "#";
    // channelName += parameter;

     if (channelName [0] != '#')
        channelName += '#';
    std::cout<<"joinchannel name:" << channelName<< std::endl;   
    std::cout<<"keyvalue:" << keyvalue<< std::endl;  


    if (server.getChannels().find(channelName) == server.getChannels().end())
        server.createChannel(*this, channelName);

    std::map<std::string, Channel>::iterator    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, channelName));
        return ;
    }
    Channel &channel = channelIt->second;
//ajouter le check si mode i n existe pas, de check si on a une key active
    try {
        if (channel.getIMode() && !channel.isInvited(this->nickName))
            server.sendTemplate(*this, ERR_INVITEONLYCHAN(this->serverName, this->nickName, channel.getName()));
        else if (!channel.getIMode() && channel.getKMode() && keyvalue == "" )
        {
            server.sendTemplate(*this, ERR_BADCHANNELKEY(this->serverName, this->nickName, channel.getName()));
        }
        else
        {
            channel.addUser(*this);
            server.replyJoin(*this, channel);
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}
//ajouter check de key si modeI non active 
//ajouter le check du nbre utilisateur si mode l active.