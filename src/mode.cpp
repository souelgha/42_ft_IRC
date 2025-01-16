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

static void insertNewMode(std::multimap<std::string, std::string> &mode, char sign, char sent, std::istringstream &parameter) {

    std::string key;
    std::string value;
    std::string check;

    key = std::string(1, sign) + sent;
    if (sent == 'i' || sent == 'k' || sent == 'l' || sent == 'o' || sent == 't')
    {
        check = std::string("+") + sent;
        if (mode.find(check) != mode.end())
            return ;
        check = std::string("-") + sent;
        if (mode.find(check) != mode.end())
            return ;
    }
    if (sent == 'k' || sent == 'l' || sent == 'o')
        parameter >> value;
    else if(sent == 'i' || sent == 't')
        value = "";
    else
        value = "unknown";
    mode.insert(std::make_pair(key, value));
}

static void adjustMode(std::multimap<std::string, std::string> &mode, std::string &value) {

    std::istringstream                      parameter(value);
    std::string                             keys;
    size_t                                  i = 0;

    parameter >> keys;
    while (keys[i])
    {
        char    sign;
        if (keys[i] == '+')
            sign = '+';
        else
            sign = '-';
        i++;
        for (; keys[i] && keys[i] != '+' && keys[i] != '-'; i++)
            insertNewMode(mode, static_cast<char>(sign), keys[i], parameter);
    }
}

void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream                      datas(parameter);
    std::string                             recipient;
    std::string                             value;
    std::multimap<std::string, std::string> mode;

    datas >> recipient;
    std::getline(datas >> std::ws, value);
    adjustMode(mode, value);
    try {
        if (server.getChannels().find(recipient) == server.getChannels().end())
        {
            if (this->mode.empty())
            {
                this->setMode("+i");
                server.replyModeClient(*this);
            }
            else
                server.sendTemplate(*this, ERR_NOSUCHCHANNEL(this->serverName, this->nickName, recipient));
        }
        else
        {
            std::cout << "Mode:" << std::endl;
            for (std::multimap<std::string, std::string>::iterator it = mode.begin(); it != mode.end(); it++)
                std::cout << it->first << " = " << it->second << std::endl;
            Channel &channel = server.getChannels()[recipient];
            if (value.empty())
                server.sendTemplate(*this, RPL_CHANNELMODEIS1(this->serverName, this->nickName, channel.getName(), channel.stringMode()));
            else if (channel.isOperator(this->nickName))
                server.replyModeChannel(*this, channel, "");
            else
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
                    this->invited.clear();
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

std::string const   Channel::stringMode(void) const {

    std::string mode = "+";

    for (std::set<char>::iterator it = this->mode.begin(); it != this->mode.end(); it++)
        mode += *it;

    return(mode);
}
