#include"Channel.hpp"

Channel::Channel(void) : name("default"), topic(""), iMode(false), tMode(false) {

    // std::fill(mode, mode + 5, 0);
}

Channel::Channel(std::string const &name) : name(name), topic(""), iMode(false), tMode(false) {

    // std::fill(mode, mode + 5, 0);
}

Channel::~Channel(void) { }

std::string const   &Channel::getName(void) const {

    return(this->name);
}

std::string const   &Channel::getTopic(void) const {

    return(this->topic);
}

std::string const   &Channel::getKey(void) const {

    return(this->key);
}

bool    Channel::getIMode(void) const {

    return(this->iMode);
}

bool    Channel::getTMode(void) const {

    return(this->tMode);
}

std::vector<Client> const   &Channel::getUsers(void) const {

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

void    Channel::addUser(Client const &client) {

    users.push_back(client);
}

void    Channel::addOper(std::string const &nickname) {

    operators.insert(nickname);
}

void    Channel::addInvited(std::string const &nickname) {

    invited.insert(nickname);
}

void    Channel::remUser(Client const &client) {

    for (std::vector<Client>::iterator it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->getNickName() == client.getNickName())
        {
            this->users.erase(it);
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

    if (this->modeSet.find('i') == this->modeSet.end())
        return ;
    invited.erase(nickname);
}

bool    Channel::isUser(std::string const &nickname) {

    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i].getNickName() == nickname)
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
