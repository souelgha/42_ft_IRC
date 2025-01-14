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
        char                    mode[5];   // i k l o t
        std::vector<Client>     users;
        std::set<std::string>   operators;
    
    public:

        Channel(std::string const &channelName);
        Channel();
        ~Channel();

        /* GETTERS */
        std::string const           &getName() const;
        std::string const           &getTopic() const;
        char const                  *getMode() const;
        std::vector<Client> const   &getUsers(void) const;
        std::set<std::string> const &getOpers(void) const;

        /* SETTERS */
        void                        setTopic(std::string const &newTopic);
        void                        setMode(std::string const &mode);

        std::string const           convertMode(void);

        void                        AddUser(Client const &client);
        void                        RemUser(Client const &client);
        void                        AddOper(std::string const &nickname);
        void                        RemOper(std::string const &nickname);
        bool                        IsUser(std::string const &nickname);
        bool                        isOperator(std::string const &nickname);
        
};
