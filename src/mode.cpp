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
        // channel.applyMode(*this);
        channel.applyMode();
        message = RPL_CHANNELMODEIS1(client.getSourceName(), client.getNickName(), channel.getName(), mode);
    }
    else
        message = RPL_CHANNELMODE(client.getServerName(), client.getNickName(), channel.getName(), mode);

    sendTemplate(client, message);
}

void Channel::insertNewMode(char sign, char sent, std::istringstream &parameter) {

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
    //ajouter si on ne trouve pas le mode ou qu on le gere pas, unknown mode : error 472. 
    // ajouter les arguments a recuperer en plus dans mode 
}

void Channel::adjustMode(std::string &value) {

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
            insertNewMode(static_cast<char>(sign), keys[i], parameter);
    }
    std::cout << "Mode:" << std::endl;
            for (std::map<std::string, std::string>::iterator it = mode.begin(); it != mode.end(); it++)
                std::cout << it->first << " = " << it->second << std::endl;
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
            channel.adjustMode(value);
            if (value.empty())
                server.sendTemplate(*this, RPL_CHANNELMODEIS1(this->serverName, this->nickName, channel.getName(), channel.stringMode()));
            else if (channel.isOperator(this->nickName))
            {

                server.replyModeChannel(*this, channel, "");
            }
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
            modeI(it);
        }
        else if(it->first == "+t" || it->first == "-t")
        {
            modeT(it);
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
            modeO(it);
        }        
    }
    
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



std::string const   Channel::stringMode(void) const {

    // std::string mode = "+";

    // for (std::set<char>::iterator it = this->mode.begin(); it != this->mode.end(); it++)
    //     mode += *it;

    return("tott");
}

