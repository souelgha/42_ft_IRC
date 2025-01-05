#include "Client.hpp"

Client::Client()
{
    
}
Client::~Client(){}
void Client::setFd(int fd)
{
    this->cliFd = fd;
}
void Client::setIpAdd(std::string addr)
{
    this->cliAddr = addr;
}
int Client::getFd() const
{
    return(this->cliFd);
}
std::string Client::getIpAddr() const
{
    return(this->cliAddr);
}