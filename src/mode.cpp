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

static bool isDuplicate(std::vector<std::pair<std::string, std::string> > &mode, char sent) {

    for (std::vector<std::pair<std::string, std::string> >::iterator it = mode.begin(); it != mode.end(); it++)
    {
        if (it->first[1] == sent)
            return (true);
    }
    return (false);
}

void    Channel::insertNewMode(Server &server, Client &client, char sign, char sent, std::istringstream &parameter) {

    std::string key;
    std::string value;

    key = std::string(1, sign) + sent;
    std::cout<< "insertkey:" << key<< std::endl;
    if (sent == 'i' || sent == 'k' || sent == 'l' || sent == 'o' || sent == 't')
    {
        if (isDuplicate(mode, sent))
            return ;
    }
    if (sent == 'k' || sent == 'o')
    {
        parameter >> value;
        std::cout<< "dans le if insertkey:" << key<< "_value:"<< value<<std::endl;
        if ((sign == '+' && (value.empty()))
            || (sign == '-' && (sent == 'k' && value.empty() && this->oldkey.empty())))
            {
                std::cout<< "dans le 2e if :insertkey:" << key<< " _value:"<< value<< " oldvalue: "<< this->oldkey<<std::endl;
                return ;
            }
            
        if (sent == 'o' && !this->isUser(value))
        {
            server.sendTemplate(client, ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickName(), this->name, value));
            if (!server.isClient(value))
                server.sendTemplate(client, ERR_NOSUCHNICK(client.getServerName(), client.getNickName(), value));
            return ;
        }
        if (sign == '-' && sent == 'k')
        {
            value = this->oldkey;
            std::cout<< "dans le 3e if :insertkey:" << key<< "_value:"<< value<< " oldvalue: "<< this->oldkey<<std::endl;
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
    mode.push_back(std::make_pair(key, value));
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
            std::cout << "fin adjmode_key[i] =  " <<key[i] << std::endl;
        }
        std::cout << "fin du for adjmode .key[i]=" << key[i]  << std::endl;
    }
     std::cout << "sortie d adjmode .key[i]=" <<std::endl;
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
            std::cout << "value: " << value << std::endl; 
            channel.adjustMode(server, *this, value); 
            std::cout << "value apres adjust: " << value << std::endl; 
            if (value.empty()){
                server.sendTemplate(*this, RPL_CHANNELMODE(this->serverName, this->nickName, channel.getName(), "+"));
                std::cout << "ici command mode: " << value << std::endl; 
            }
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
   std::vector<std::pair<std::string, std::string> >::iterator it;
    for (it = mode.begin() ; it != mode.end(); it++)
    {
        if(it->first == "+i" || it->first == "-i")
            modeI(it);
        else if(it->first == "+t" || it->first == "-t")
            modeT(it);
        else if(it->first == "+k" || it->first == "-k")
            modeKey(it);
        else if(it->first == "+l" || (it->first == "-l"))
            modeL(it);
        else if(it->first == "+o" || it->first == "-o")
            modeO(it);
    }
}

std::vector<std::pair<std::string, std::string> > const    &Channel::getMode() const {

    return (this->mode);
}

void Channel:: modeKey(std::vector<std::pair<std::string, std::string> >::iterator &it) 
{
    if(it->first == "+k")
    {
        if(it->second != "")
        {
            this->key = it->second;
            this->oldkey = it->second;
            this->kMode = true;
        }
        
    }        
    else if(it->first == "-k")
    {        
        this->key = "";
        this->kMode = false;
    }
    //ajout dans join et check si mode i active
    std::cout<<"modekey value : "<< kMode<< " ; keyvalue "<< this->key<< std::endl; 
}

void Channel:: modeL(std::vector<std::pair<std::string, std::string> >::iterator &it) 
{
    if(it->first == "+l" && it->second!= "")
    {
        char const *val = ((it->second).c_str());
        this->limitUsers = std::atoi(val);
        this->lMode = true;
        //si it->second == "" renvoyer => ERROR 461
    }        
    else if(it->second == "-l") 
    {
        this->limitUsers = 10000;
        this->lMode = false;
    }
    //renvoyer ERROR 471 dans Join si channel is full
    
}

void Channel:: modeI(std::vector<std::pair<std::string, std::string> >::iterator &it) 
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
void Channel:: modeT(std::vector<std::pair<std::string, std::string> >::iterator &it) 
{
    if(it->first == "+t")
    {
        this->tMode = true;
    }        
    else if(it->first == "-t")
    {
        this->tMode = false;
    }
    
}

void Channel:: modeO(std::vector<std::pair<std::string, std::string> >::iterator &it) 
{
    if(it->first == "+o" && it->second != "")
    {  
        for (size_t i = 0; i < users.size(); i++)
        {
            if(users[i].getNickName() == it->second)
            {
                addOper(it->second); 
                break;
            }
        }
    // voir ou est gere l erreur si c est dans la fonction mode ou la mettre ici
        //ajouter le user dans oper et cehck de l affichage       
    }        
    else if(it->first == "-o" && it->second != "")
    {
        if(operators.find(it->second) != operators.end())
        {
            remOper(it->second);
        }
        else
        {
            //message d erreur 
                   /*tt: No such nick/channel
        19:19 -!- tt #42 They aren't on that channel
        error 401 et 441
        ERR_NOSUCHNICK
        ERR_USERNOTINCHANNEL*/        
        //retirer user dans op
        }
    }
    
}

std::string const   Channel::stringMode(void) {

   std::vector<std::pair<std::string, std::string> >::iterator    it = this->mode.begin();
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
    std::cout << "stringMode pour modechannel: "<< mode<<std::endl;
    return(mode);
}

