#include"Channel.hpp"

Channel::Channel(std::string const &channelName)
{
    this->name = channelName;
    this->topic = "";
}

std::string Channel::getName() const
{
    return(this->name);
}
std::string Channel::getTopic() const
{
    return(this->topic);
}
void Channel::setTopic(std::string  const &newTopic)
{
    this->topic = newTopic;
}

void Channel::AddUser(std::string const &nickname)
{
    users.insert(nickname);
}
void Channel::RemUser(std::string const &nickname)
{
    users.erase(nickname);
}
void Channel::AddOper(std::string const &nickname)
{
    operators.insert(nickname);
}
void Channel::RemOper(std::string const &nickname)
{
    operators.erase(nickname);
}

bool Channel::IsUser(std::string const &nickname)
{
    return(users.find(nickname) != users.end());
        
}
bool Channel::IsOperator(std::string const &nickname)
{
    return(operators.find(nickname) != operators.end());
}
std::vector <std::string> Channel::getUsers() const
{
    return(std::vector<std::string>(users.begin(), users.end()));
}