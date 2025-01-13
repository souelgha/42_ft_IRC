#pragma once

#include <map>
#include <set>
#include <iostream>
#include <vector>

#include "Client.hpp"

class   Client;

class Channel
{
    private:
        std::string             name;
        std::string             topic;
        char                    mode[5];   // i t o k l
        std::vector<Client>     users;
        std::set<std::string>   operators;
    
    public:
        Channel(std::string const &channelName);
        Channel();
        ~Channel();

        std::string getName() const;
        std::string getTopic() const;
        char const  *getMode() const;

        void setTopic(std::string const &newTopic);
        void setMode(std::string const &mode);

        void AddUser(Client &client);
        void RemUser(Client &client);

        void AddOper(std::string const &nickname);
        void RemOper(std::string const &nickname);

        bool IsUser(std::string const &nickname);
        bool isOperator(std::string const &nickname);
        
        std::vector<Client> const   &getUsers(void) const;
        std::set<std::string> const &getOpers(void) const;
};
