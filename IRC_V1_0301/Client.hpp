#pragma once

#include<iostream>

class Client
{
    private:
        int cliFd; //fd du client
        std::string cliAddr; //addre socket client
    public:
        Client();
        ~Client();
        void setFd(int fd);
        void setIpAdd(std::string addr);
        int getFd() const;
        std::string getIpAddr() const;
};