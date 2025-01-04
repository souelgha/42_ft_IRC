#pragma once

#include<iostream>

class Client
{
    private:
        int cliFd;
        std::string cliAddr;
    public:
        Client();
        ~Client();
        void setFd(int fd);
        void setIpAdd(std::string add);
        int getFd() const;
};