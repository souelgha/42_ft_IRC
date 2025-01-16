#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void    Server::replyModeClient(Client const &client) {

    std::string const message = RPL_UMODEIS(client.getServerName(), client.getNickName(), client.getMode());
    sendTemplate(client, message);
}

void    Server::replyModeChannel(Client const &client, Channel &channel, std::string const &mode) {

    std::string message = "";

    if (!mode.empty())
    {
        channel.setMode(mode);
        message = RPL_CHANNELMODEIS1(client.getSourceName(), client.getNickName(), channel.getName(), mode);
    }
    else
        message = RPL_CHANNELMODE(client.getServerName(), client.getNickName(), channel.getName(), mode);

    sendTemplate(client, message);
}

std::string  adjustMode(std::string &parameter) {

    std::string mode = "";
    size_t      i = 0;

    if (parameter[i] != '+' && parameter[i] != '-')
        return (mode);
    mode += parameter[i];
    i++;
    for (; parameter[i] && (parameter[i] == 'i' || parameter[i] == 'k' || parameter[i] == 'l' || parameter[i] == 'o' || parameter[i] == 't'); i++)
        mode += parameter[i];
    return (mode);
}

void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream  datas(parameter);
    std::string         recipient;
    std::string         value;
    std::string         mode;

    datas >> recipient;
    datas >> value;
    mode = adjustMode(value);
    try {
        if (server.getChannels().find(recipient) == server.getChannels().end())
        {
            std::cout << "mode client" << std::endl;
            this->setMode(mode);
            server.replyModeClient(*this);
        }
        else
        {
            std::cout << "mode channel" << std::endl;
            Channel &channel = server.getChannels()[recipient];
            if (channel.isOperator(this->nickName))
                server.replyModeChannel(*this, channel, mode);
            else if (!value.empty())
                server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel.getName()));
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::set<char> const    &Channel::getMode() const {

    return (this->mode);
}

void    Channel::setMode(std::string const &mode) {

    for (size_t i = 0; mode[i]; i++)
    {
        if (mode[i] == '+')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                    this->mode.insert('i');
                else if (mode[i] == 'l')
                    this->mode.insert('l');
                else if (mode[i] == 'o')
                    this->mode.insert('o');
                else if (mode[i] == 't')
                    this->mode.insert('t');
            }
        }
        if (mode[i] == '-')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                {
                    this->mode.erase('i');
                    // clear liste invites
                }
                else if (mode[i] == 'k')
                    this->key.clear();
                else if (mode[i] == 'l')
                    this->mode.erase('i');
                else if (mode[i] == 'o')
                    this->mode.erase('i');
                else if (mode[i] == 't')
                    this->mode.erase('i');
            }
        }
    }
}

// std::string const   Channel::convertMode(void) const {

//     std::string mode = "+";

//     if (this->mode[0])
//         mode += "i";
//     if (this->mode[1])
//         mode += "k";
//     if (this->mode[2])
//         mode += "l";
//     if (this->mode[3])
//         mode += "o";
//     if (this->mode[4])
//         mode += "t";

//     return(mode);
// }
