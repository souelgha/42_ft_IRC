#include"Channel.hpp"

Channel::Channel(void) {

    this->name =  "default";
    this->key = "";
    this->oldkey = "";
    this->topic = "";
    this->iMode = false;
    this->tMode = false;
    this->lMode = false;
    this->kMode = false;
    this->limitUsers = 10000;

}

Channel::Channel(std::string const &name){

    this->name = name;
    this->key = "";
    this->oldkey = "";
    this->topic = "";
    this->iMode = false;
    this->tMode = false;
    this->lMode = false;
    this->kMode = false;
    this->limitUsers = 10000;

}

Channel::~Channel(void) { }

std::string const   &Channel::getName(void) const {

    return(this->name);
}

std::string const   &Channel::getTopic(void) const {

    return(this->topic);
}

std::string const   Channel::getKey(void) const {

    return(this->key);
}

bool    Channel::getIMode(void) const {

    return(this->iMode);
}

bool    Channel::getTMode(void) const {

    return(this->tMode);
}
bool    Channel::getLMode(void) const {

    return(this->lMode);
}
bool    Channel::getKMode(void) const {

    return(this->kMode);
}

size_t    Channel::getLimitUsers(void) const{
    
    return(this->limitUsers);
 }

std::string const   &Channel::getStringMode(void) const {

    return (this->stringMode);
}

std::vector<Client *> const   &Channel::getUsers(void) const {

    return (this->users);
}

std::set<std::string> const &Channel::getOpers(void) const {

    return (this->operators);
}

std::set<std::string> const &Channel::getInvited(void) const {

    return (this->invited);
}

void    Channel::setTopic(std::string const &topic) {

    this->topic = topic;
}

void    Channel::setKey(std::string const &key) {

    this->key = key;
}

void    Channel::setIMode(bool mode) {

    this->iMode = mode;
}

void    Channel::setTMode(bool mode) {

    this->tMode = mode;
}
void    Channel::setKMode(bool mode) {

    this->kMode = mode;
}
void    Channel::setStringMode(void) {

    this->stringMode = "+";

    if (this->tMode)
        this->stringMode += "t";
    if (this->iMode)
        this->stringMode += "i";
    if (this->lMode)
        this->stringMode += "l";
    if (this->kMode)
        this->stringMode += "k";
    if (this->lMode)
    {
        std::stringstream limit;
        limit << this->limitUsers;
        this->stringMode += " " + limit.str();
    }
    if (this->kMode)
        this->stringMode += " " + this->key;
}

void    Channel::addUser(Client * client) {

    users.push_back(client);
}

void    Channel::addOper(std::string const &nickname) {

    operators.insert(nickname);
}

void    Channel::addInvited(std::string const &nickname) {

    invited.insert(nickname);
}

void    Channel::remUser(Client const &client) {

    for (size_t it = 0; it < this->users.size(); it++)
    {
        if (this->users[it]->getNickName() == client.getNickName())
        {
            this->users.erase(this->users.begin() + it);
            break ;
        }
    }
}

void    Channel::remOper(std::string const &nickname) {

    if (!isOperator(nickname))
        return ;
    operators.erase(nickname);
}

void    Channel::remInvited(std::string const &nickname) {

    if (this->iMode == false)
        return ;
    invited.erase(nickname);
}

bool    Channel::isUser(std::string const &nickname) {

    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i]->getNickName() == nickname)
            return (true);
    }
    return (false);
}

bool    Channel::isOperator(std::string const &nickname) {

    return (operators.find(nickname) != operators.end());
}

bool    Channel::isInvited(std::string const &nickname) {

    return (invited.find(nickname) != invited.end());
}
