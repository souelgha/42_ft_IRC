#pragma once

#include<map>
#include<set>
#include<iostream>
#include<vector>

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::set<std::string> users;
        std::set<std::string> operators;
    
    public:
        Channel(std::string const &channelName);
        Channel();
        ~Channel();

        std::string getName() const;
        std::string getTopic() const;

        void setTopic(std::string  const &newTopic);

        void AddUser(std::string const &nickname);
        void RemUser(std::string const &nickname);

        void AddOper(std::string const &nickname);
        void RemOper(std::string const &nickname);

        bool IsUser(std::string const &nickname);
        bool IsOperator(std::string const &nickname);
        
        std::vector <std::string> getUsers() const;
        std::vector <std::string> getOpers() const;



};
