#include"Channel.hpp"

Channel::Channel(void) : name("default"), topic("") { }

Channel::~Channel(void) { }

Channel::Channel(std::string const &channelName) {

    this->name = channelName;
    this->topic = "";
}

std::string Channel::getName(void) const {

    return(this->name);
}

std::string Channel::getTopic(void) const {

    return(this->topic);
}

void    Channel::setTopic(std::string const &newTopic) {

    this->topic = newTopic;
}

void    Channel::AddUser(Client &client) {

    users.push_back(client);
}

void    Channel::RemUser(Client &client) {

    for (std::vector<Client>::iterator it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->getNickName() == client.getNickName())
        {
            this->users.erase(it);
            break ;
        }
    }
}

void    Channel::AddOper(std::string const &nickname) {

    operators.insert(nickname);
    std::cout << nickname << " is operator!" << std::endl;
}

void    Channel::RemOper(std::string const &nickname) {

    operators.erase(nickname);
}

bool    Channel::IsUser(std::string const &nickname) {

    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i].getNickName() == nickname)
            return (true);
    }
    return (false);
}

bool Channel::IsOperator(std::string const &nickname) {

    return (operators.find(nickname) != operators.end());
}

std::vector<Client> const   &Channel::getUsers(void) const {

    return (this->users);
}

std::set<std::string> const &Channel::getOpers(void) const {

    return (this->operators);
}
