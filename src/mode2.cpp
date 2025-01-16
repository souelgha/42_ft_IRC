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

    // std::istringstream  datas(parameter);
    // std::string         modetype;
    // std::string         args;

    // datas >> modetype;
    // datas >> args;
    // std::cout<< "modetype:<" << modetype <<">"<< std::endl;
    // std::cout<< "args:<" << args <<">"<< std::endl;

    std::string mode = "";
    size_t      i = 0;

    if (parameter[i] != '+' && parameter[i] != '-')
        return (mode);
    mode += parameter[i];
    i++;
    for (; parameter[i] && (parameter[i] == 'i' || parameter[i] == 'k' || parameter[i] == 'l' || parameter[i] == 'o' || parameter[i] == 't'); i++)
        mode += parameter[i];
    //ajouter si on ne trouve pas le mode ou qu on le gere pas, unknown mode : error 472. 
    // ajouter les arguments a recuperer en plus dans mode 
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
    std::cout<<"value<"<<value<< ">"<<  std::endl;
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
                else if (mode[i] == 't')
                    this->mode.insert('t');
                else if (mode[i] == 'k')
                {
                    this->mode.insert('k');
                    
                }

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
                {
                    this->mode.erase('k');
                    // afficher dans interface [ -k pwd]
                    this->key.clear();
                }
                else if (mode[i] == 't')
                    this->mode.erase('t');
                // else if (mode[i] == 'l')
                //     //fnct retirer la limite utilisateur;
                // else if (mode[i] == 'o')
                //     //fnct retirer les droits operateurs
                
            }
        }
    }
    // si plusieurs commandes afficher 
    // std::set<char>::iterator it;
    // for(it = this->mode.begin(); it != this->mode.end(); it++)
    //  std::cout<< *it << std::endl;
}


/*  mode +/- k : key channel */
/* check du privilege du user => si pas oper :
        => renvoie erreur
        ERR_CHANOPRIVSNEEDED(482) dans le channel pas dans l interface
        => ne fait rien, pas de chgt.
        +k => ajoute une key. il faut d abord remove la key avant de la modifier.
    on recoit parameters
        recipient => channel
        value => mode
        
    typo attendu pour k
        =>  /mode #channel +k pwd ===> ajout key
        => ou mode #channel -k ===> retrait de la key
        +> /mode #channel -k+k newkey
        => si +k ss argument => ne fait rien et ne renvoie pas d erreur
        => si +k avec plusieurs commandes 
    retour vers tous les  clients avec la reponse 324 dans le channel : 
        => si seule : [+k pwd] ou [-k pwd] 
        => si plusieurs commmandes on ajoute les commandes dans l ordre recu avec les arg si besoin
            [-ik hello] [+ok dfg hello]
            on retire les signes en plus lors du renvoie.
    si des arguments sont manquants ex : mode -o-k hello  ou hello est la key du channel
        hello: No such nick/channel
        hello #wert They aren't on that channel
    join #channel ss key, on renvoie l erreur
       =>   Cannot join to channel #wert (Bad channel key)
    mode -o+k he : 
        => he: No such nick/channel
        => he #wert They aren't on that channel . 
        ne change pas la key.
        on doit d abord faire -k avant de modifier la clef
    /mode +k+o he:
        => chg la key
        => ne met pas d erreur sur le +o



 */

void ModeKey(){}
