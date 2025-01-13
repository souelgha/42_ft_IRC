#include"Channel.hpp"

Channel::Channel(void) : name("default"), topic("") {

    std::fill(mode, mode + 5, 0);
}

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

char const  *Channel::getMode() const {

    return (this->mode);
}

void    Channel::setTopic(std::string const &newTopic) {

    this->topic = newTopic;
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

bool Channel::isOperator(std::string const &nickname) {

    return (operators.find(nickname) != operators.end());
}

std::vector<Client> const   &Channel::getUsers(void) const {

    return (this->users);
}

std::set<std::string> const &Channel::getOpers(void) const {

    return (this->operators);
}
