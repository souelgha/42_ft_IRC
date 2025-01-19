#pragma once

#include <map>
#include <set>
#include <iostream>
#include <vector>

#include "Server.hpp"
#include "Client.hpp"

class   Server;
class   Client;

class Channel
{
    private:

        std::string                                 name;
        std::vector<std::pair<std::string, std::string> >     mode;
        // std::set<char>                          modeSet;   // i k l o t

        std::string             topic;
        std::string             key;
        std::string             oldkey;
        bool                    iMode;
        bool                    kMode;
        bool                    tMode;
        bool                    lMode;
        size_t                  limitUsers;

        std::vector<Client>     users;
        std::set<std::string>   operators;
        std::set<std::string>   invited;
    
    
    public:

        /* CONSTRUCTORS */
        Channel(void);
        Channel(std::string const &channelName);
        // Channel(const Channel &other);
        // Channel &operator=(const Channel &other);

        /* DESTRUCTORS */
        ~Channel(void);

        /* GETTERS */
        std::string const                               &getName(void) const;
        std::vector<std::pair<std::string, std::string> >  const        &getMode(void) const;
        std::set<char> const                            &getModeSet(void) const;
        std::string const                               &getTopic(void) const;
        std::string const                               getKey(void) const;
        bool                                            getIMode(void) const;
        bool                                            getTMode(void) const;
        bool                                            getLMode(void) const;
        bool                                            getKMode(void) const;
        size_t                                             getLimitUsers(void) const;
        std::vector<Client> const                       &getUsers(void) const;
        std::set<std::string> const                     &getOpers(void) const;
        std::set<std::string> const                     &getInvited(void) const;

        /* SETTERS */
        void                                            setMode(std::string const &mode);
        void                                            setTopic(std::string const &topic);
        void                                            setKey(std::string const &key);
        void                                            setIMode(bool mode);
        void                                            setTMode(bool mode);
        void                                            setKMode(bool mode);

        /* OPERATIONS */
        void                                            addUser(Client const &client);
        void                                            addOper(std::string const &nickname);
        void                                            addInvited(std::string const &nickname);
        void                                            remUser(Client const &client);
        void                                            remOper(std::string const &nickname);
        void                                            remInvited(std::string const &nickname);
        bool                                            isUser(std::string const &nickname);
        bool                                            isOperator(std::string const &nickname);
        bool                                            isInvited(std::string const &nickname);

        /* GESTION DES MODES */
        // void                        modeKey(std::string const &value);
        void                        applyMode(void) ;
        void                        modeKey(std::vector<std::pair<std::string, std::string> > ::iterator &it);
        void                        modeL(std::vector<std::pair<std::string, std::string> > ::iterator &it);
        void                        modeI(std::vector<std::pair<std::string, std::string> > ::iterator &it);
        void                        modeO(std::vector<std::pair<std::string, std::string> > ::iterator &it);
        void                        modeT(std::vector<std::pair<std::string, std::string> >::iterator &it);
        
        
        
        /* UTILS */
        std::string const                               stringMode(void);
        void                                            adjustMode(Server &server, Client &client, std::string &value);
    void                                                insertNewMode(Server &server, Client &client, char sign, char sent, std::istringstream &parameter);
};
