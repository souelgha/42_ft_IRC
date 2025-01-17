#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void    Server::replyModeClient(Client const &client) {

    std::string const message = RPL_UMODEIS(client.getServerName(), client.getNickName(), client.getMode());
    sendTemplate(client, message);
}

void    Server::replyModeChannel(Client const &client, Channel &channel, std::string const &mode) {

    std::string message = "";

    std::cout << "In reply mode channel, mode = " << mode << std::endl;
    if (!mode.empty())
    {
        channel.applyMode();   
        message = RPL_CHANNELMODEIS1(client.getSourceName(), channel.getName(), mode);
    }
    else
        message = RPL_CHANNELMODE(client.getServerName(), client.getNickName(), channel.getName(), mode);

    for (size_t i = 0; i < channel.getUsers().size(); i++)
        sendTemplate(channel.getUsers()[i], message);
}

static bool isDuplicate(std::map<std::string, std::string> &mode, char sent) {

    std::string check;

    check = std::string("+") + sent;
    if (mode.find(check) != mode.end())
        return (true);
    check = std::string("-") + sent;
    if (mode.find(check) != mode.end())
        return (true);
    return (false);
}

void    Channel::insertNewMode(Server &server, Client &client, char sign, char sent, std::istringstream &parameter) {

    std::string key;
    std::string value;

    key = std::string(1, sign) + sent;
    if (sent == 'i' || sent == 'k' || sent == 'l' || sent == 'o' || sent == 't')
    {
        if (isDuplicate(mode, sent))
            return ;
    }
    if (sent == 'k' || sent == 'o')
    {
        parameter >> value;
        if ((sign == '+' && (value.empty()))
            || (sign == '-' && (sent == 'k' && value.empty() && this->key.empty())))
            return ;
        if (sent == 'o' && !this->isUser(value))
        {
            server.sendTemplate(client, ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickName(), this->name, value));
            if (!server.isClient(value))
                server.sendTemplate(client, ERR_NOSUCHNICK(client.getServerName(), client.getNickName(), value));
            return ;
        }
    }
    else if (key == "+l")
    {
        parameter >> value;
        if (value.empty())
        {
            server.sendTemplate(client, ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickName(), key));
            return ;
        }
        else if (std::atoi(value.c_str()) == 0)
            return ;
    }
    else if (sent == 'i' || sent == 't' || key == "-l")
        value = "";
    std::cout << "mode inserted: key = " << key << ", value = " << value << std::endl;
    mode.insert(std::make_pair(key, value));
}

void    Channel::adjustMode(Server &server, Client &client, std::string &value) {

    std::string         keys;
    size_t              i = 0;

    if (value.empty() || (value[0] != '+' && value[0] != '-'))
    {
        value.clear();
        return ;
    }

    std::istringstream  parameter(value);
    parameter >> keys;
    while (keys[i])
    {
        char    sign;
        if (keys[i] == '-')
            sign = '-';
        else
            sign = '+';
        i++;
        for (; keys[i] && keys[i] != '+' && keys[i] != '-'; i++)
        {
            if (keys[i] != 'i' && keys[i] != 'k' && keys[i] != 'l' && keys[i] != 'o' && keys[i] != 't')
                server.sendTemplate(client, ERR_UNKNOWNMODE(client.getServerName(), client.getNickName(), keys[i], this->name));
            insertNewMode(server, client, static_cast<char>(sign), keys[i], parameter);
        }
    }
}

void    Client::commandMode(Server &server, std::string const &parameter) {

    std::istringstream                      datas(parameter);
    std::string                             recipient;
    std::string                             value;


    datas >> recipient;
    std::getline(datas >> std::ws, value);
   
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
            Channel &channel = server.getChannels()[recipient];
            channel.adjustMode(server, *this, value);
            if (value.empty())
                server.sendTemplate(*this, RPL_CHANNELMODE(this->serverName, this->nickName, channel.getName(), "+"));
            else if (channel.isOperator(this->nickName))
                server.replyModeChannel(*this, channel, channel.stringMode());
            else
                server.sendTemplate(*this, ERR_CHANOPRIVSNEEDED(this->serverName, this->nickName, channel.getName()));
        }
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Channel::applyMode(void) 
{
    
    std::map<std::string, std::string>::iterator it;
    for (it = mode.begin() ; it != mode.end(); it++)
    {
        if(it->first == "+i" || it->first == "-i")
        {
            // modeI(it);
        }
        else if(it->first == "+t" || it->first == "-t")
        {
            // modeT(it);
        }
        else if(it->first == "+k" || it->first == "-k")
        {
            modeKey(it);
        }
        else if(it->first == "+l" || (it->first == "-l"))
        {
            modeL(it);
        }
        else if(it->first == "+o" || it->first == "-o")
        {
            // modeO(it);
        }        
    }
}

std::map<std::string, std::string> const    &Channel::getMode() const {

    return (this->mode);
}

void Channel:: modeKey(std::map<std::string, std::string>::iterator &it) 
{
    if(it->first == "+k")
    {
        if(it->second != "")
            this->key = it->second;
    }        
    else if(it->first == "-k")
    {
        this->key = "";
    }
}

void Channel:: modeL(std::map<std::string, std::string>::iterator &it) 
{
    if(it->first == "+l" && it->second!= "")
    {
        char const *val = ((it->second).c_str());
        this->limitUsers = std::atoi(val);
        //si it->second == "" renvoyer => ERROR 461
    }        
    else 
    {
        this->limitUsers = 10000;
    }
    //renvoyer ERROR 471 dans Join si channel is full
    
}

void Channel:: modeI(std::map<std::string, std::string>::iterator &it) 
{
    if(it->first == "+i")
    {
        this->iMode = true;
    }        
    else if(it->first == "-i")
    {
        this->iMode = false;
    }
   // si iMode == true => le +k ne sert pas 
   // si IMode = false => +k necessaire.
    
}

std::string const   Channel::stringMode(void) {

    std::map<std::string, std::string>::iterator    it = this->mode.begin();
    std::string                                     mode = it->first;
    char                                            sign = it->first[0];

    it++;
    for (; it != this->mode.end(); it++)
    {
        if (it->first[0] == sign)
            mode += it->first[1];
        else
        {
            mode += it->first;
            sign = it->first[0];
        }
    }
    for (it = this->mode.begin(); it != this->mode.end(); it++)
    {
        if (!it->second.empty())
            mode += " " + it->second;
    }
    return(mode);
}

