#pragma once

#include <map>
#include <set>
#include <iostream>
#include <vector>

#include "Client.hpp"

enum	e_mode
{
	INVITE_ONLY = 0,
	KEY = 1,
	TOPIC_RESTRICTION = 4,
};

class   Client;

class Channel
{
    private:

        std::string             name;
        std::set<char>          mode;   // i k l o t

        std::string             topic;
        std::string             key;

        std::vector<Client>     users;
        std::set<std::string>   operators;
        std::set<std::string>   invited;
    
    public:

        /* CONSTRUCTORS */
        Channel(void);
        Channel(std::string const &channelName);

        /* DESTRUCTORS */
        ~Channel(void);

        /* GETTERS */
        std::string const           &getName(void) const;
        std::set<char> const        &getMode(void) const;
        std::string const           &getTopic(void) const;
        std::string const           &getKey(void) const;
        std::vector<Client> const   &getUsers(void) const;
        std::set<std::string> const &getOpers(void) const;
        std::set<std::string> const &getInvited(void) const;

        /* SETTERS */
        void                        setMode(std::string const &mode);
        void                        setTopic(std::string const &topic);
        void                        setKey(std::string const &key);

        /* OPERATIONS */
        void                        addUser(Client const &client);
        void                        addOper(std::string const &nickname);
        void                        addInvited(std::string const &nickname);
        void                        remUser(Client const &client);
        void                        remOper(std::string const &nickname);
        void                        remInvited(std::string const &nickname);
        bool                        isUser(std::string const &nickname);
        bool                        isOperator(std::string const &nickname);
        bool                        isInvited(std::string const &nickname);

        /* UTILS */
        std::string const           stringMode(void) const ;
};
