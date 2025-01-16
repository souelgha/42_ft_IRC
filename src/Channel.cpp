#include"Channel.hpp"

Channel::Channel(void) : name("default"), topic("") {

    std::fill(mode, mode + 5, 0);
}

Channel::Channel(std::string const &name) : name(name), topic("") {

    std::fill(mode, mode + 5, 0);
}

Channel::~Channel(void) { }

std::string const   &Channel::getName(void) const {

    return(this->name);
}

char const  *Channel::getMode() const {

    return (this->mode);
}

std::string const   &Channel::getTopic(void) const {

    return(this->topic);
}

std::string const   &Channel::getKey(void) const {

    return(this->key);
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

void    Channel::setMode(std::string const &mode) {

    for (size_t i = 0; mode[i]; i++)
    {
        if (mode[i] == '+')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                    this->mode[0] = 1;
                else if (mode[i] == 'k')
                    this->mode[1] = 1;
                else if (mode[i] == 'l')
                    this->mode[2] = 1;
                else if (mode[i] == 'o')
                    this->mode[3] = 1;
                else if (mode[i] == 't')
                    this->mode[4] = 1;
            }
        }
        if (mode[i] == '-')
        {
            i++;
            for (; mode[i] && isalpha(mode[i]); i++)
            {
                if (mode[i] == 'i')
                    this->mode[0] = 0;
                else if (mode[i] == 'k')
                    this->mode[1] = 0;
                else if (mode[i] == 'l')
                    this->mode[2] = 0;
                else if (mode[i] == 'o')
                    this->mode[3] = 0;
                else if (mode[i] == 't')
                    this->mode[4] = 0;
            }
        }
    }
}

void    Channel::setTopic(std::string const &topic) {

    this->topic = topic;
}

void    Channel::setKey(std::string const &key) {

    this->key = key;
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

    if (!this->mode[INVITE_ONLY])
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

std::string const   Channel::convertMode(void) const {

    std::string mode = "+";

    if (this->mode[0])
        mode += "i";
    if (this->mode[1])
        mode += "k";
    if (this->mode[2])
        mode += "l";
    if (this->mode[3])
        mode += "o";
    if (this->mode[4])
        mode += "t";

    return(mode);
}
