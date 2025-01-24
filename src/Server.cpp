/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:26:29 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/24 14:39:32 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::signal = false;

Server::Server(void) { }

Server::Server(int listening_port, std::string const &password) : name("our_IRC"), password(password), listen_port(listening_port), listen_fd(-1) {

    for (size_t i = 0; i < this->maxClients; i++)
        this->clients[i] = NULL;
    for (size_t i = 0; i < this->maxClients; i++)
        this->toDelete[i] = NULL;
}

Server::~Server(void) {

    for (std::map<std::string, Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
        delete it->second;
    for (size_t i = 0; i < this->maxClients; i++)
        delete this->clients[i];
    for (size_t i = 0; i < this->maxClients; i++)
        delete this->toDelete[i];
    if (this->listen_fd != -1)
        close (this->listen_fd);
}

size_t  Server::getMaxClients(void) const {

    return (Server::maxClients);
}

std::string const   &Server::getName(void) const {

    return (this->name);
}

std::string const   &Server::getPassword(void) const {

    return (this->password);
}

int Server::getListenPort(void) const {

    return (this->listen_port);
}

Client  **Server::getClients(void) {

    return (this->clients);
}

std::map<std::string, Channel *>    &Server::getChannels(void) {

    return (this->channels);
}

void    Server::serverInit(void)
{
    try {
        this->serverConnect();
    }
    catch (std::exception &e) {
        throw ;
    }

    std::cout
        << RED << "En attente de nouvelles connexions..." << WHITE << std::endl;
    while (Server::signal == false)
    {
        if (poll(&this->fds[0], this->fds.size(), -1) == -1)
            throw (std::runtime_error("Failed to bind poll\n"));
        for (size_t i = 0; i < this->fds.size(); i++)
        {
            if (this->fds[i].revents & POLLIN)    // si un evenement a eu lieu
            {
                try {
                    if (this->fds[i].fd == this->listen_fd)  // si cet evenement a lieu sur le port d'ecoute => ajouter un nouveau client
                        this->newClient();
                    else
                        this->receiveMessage(this->findClient(this->fds[i].fd));
                    this->deleteClients();
                }
                catch (std::exception &e) {
                    throw ;
                }
            }
        }
    }   
}

void    Server::serverConnect(void)
{
    this->listenAddress.sin_family = AF_INET;                     
    this->listenAddress.sin_port = htons(this->listen_port);      
    this->listenAddress.sin_addr.s_addr = INADDR_ANY;             
    socklen_t   listenAddressLength = sizeof(this->listenAddress);  

    this->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listen_fd == -1)
        throw (std::runtime_error("Failed to initialise socket"));

    int en = 1;
	if (setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw (std::runtime_error("Failed to set option on socket"));
    if (fcntl(this->listen_fd, F_SETFL, O_NONBLOCK) == -1) 
		throw (std::runtime_error("Failed to set option on socket"));

    // lie le socket a l'adresse
    if (bind(this->listen_fd, (struct sockaddr*) &this->listenAddress, listenAddressLength) == -1)
        throw (std::runtime_error("Failed to bind socket"));

    // commence a ecouter
    if (listen(this->listen_fd, this->maxClients) == -1)
        throw  (std::runtime_error("Failed to listen"));    

    // permet de verifier les evenements
    struct pollfd   newPoll;
    newPoll.fd = listen_fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->fds.push_back(newPoll);
}

void    Server::newClient(void)
{
    std::cout
        << BLUE << "New Client" << WHITE << std::endl;

    Client  *newClient = new Client;

    int clientAddressLength = sizeof(this->clientAddress);
    int connection_fd = accept(this->listen_fd, (struct sockaddr*) &this->clientAddress, (socklen_t *) &clientAddressLength);
    if (connection_fd == -1)
        throw (std::runtime_error("Failed to accept")); 
    if (this->fds.size() < this->maxClients)
    {
        struct pollfd   newPoll;
        newPoll.fd = connection_fd;
        newPoll.events = POLLIN;
        newPoll.revents = 0;
        this->fds.push_back(newPoll);
        newClient->setFd(connection_fd);
        newClient->setIpAddress(inet_ntoa(this->clientAddress.sin_addr));
        this->addClient(newClient);
    }
    else
    {
        std::cerr
            << "Too many clients" << std::endl;
        delete newClient;
        close (connection_fd);
    }
}

static bool incompleteCommand(char *buffer) {

    if (buffer == NULL)
        return (true);
    if (strstr(buffer, DELIMITER))
        return (false);
    return (true);
}

void Server::receiveMessage(Client *client)
{
    std::cout
        << YELLOW << "Message received (" << client->getFd() << "):" << WHITE << std::endl;

    if (client == NULL)
        return ;
    size_t  buffer_len = std::strlen(client->buffer);
    int     receivedBytes = recv(client->getFd(), client->buffer + buffer_len, BUFFER_SIZE - buffer_len, 0);
    if (receivedBytes <= 0)
    {
        if (receivedBytes == -1)
            std::cerr
                << "Failed to receive client's message" << std::endl;
        this->clearClient(client);
        return ;
    }
    if (incompleteCommand(client->buffer))
        return ;
    client->buffer[receivedBytes + buffer_len] = '\0';
    client->commandReact(*this);
}

void    Server::addClient(Client *client) {

    size_t  i = 0;

    if (client == NULL)
        return ;
    while (this->clients[i])
        i++;
    if (i == this->maxClients)
        throw (std::runtime_error("Too many clients are connected"));
    this->clients[i] = client;
}

void    Server::addToDelete(Client *client) {

    size_t  i = 0;

    if (client == NULL)
        return ;
    while (this->toDelete[i])
        i++;
    if (i == this->maxClients)
        throw (std::runtime_error("Too many clients to be deleted"));
    this->toDelete[i] = client;
}

void    Server::deleteClients(void) {

    for (size_t i = 0; i < this->maxClients; i++)
    {
        if (this->toDelete[i])
        {
            delete this->toDelete[i];
            this->toDelete[i] = NULL;
        }
    }
}

void    Server::deleteChannel(Channel *channel)
{
    if (channel == NULL)
        return ;
    if (channels.find(channel->getName()) != channels.end())
        channels.erase(channel->getName());
    delete channel;
}

void    Server::signalCatch(int) {

    signal = true;
    throw (std::runtime_error(""));
}

void    Server::clearClient(Client *client) {

    if (client == NULL)
        return ;
    for (size_t i = 0; i < this->maxClients; i++)
    {
        if (this->clients[i] && this->clients[i]->getNickName() == client->getNickName())
        {
            try {
                this->addToDelete(this->clients[i]);
                this->clients[i] = NULL;
                break;
            }
            catch (std::exception &e) {
                throw ;
            }
        }
    }
    for (size_t i = 0; i < this->fds.size(); i++)
    {
        if (this->fds[i].fd == client->getFd())
        {
            this->fds.erase(this->fds.begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < client->getListChannels().size(); i++)
    {
        try {
            client->getListChannels()[i]->remUser(*client);
            this->replyPart(*client, client->getListChannels()[i]);
        }
        catch (std::exception &e) {
            throw ;
        }
    }
    std::fill(client->buffer, client->buffer + BUFFER_SIZE, 0);
}

bool    Server::isClient(std::string const &nickname) {

    for (size_t i = 0; i < this->maxClients; i++)
    {
        if (this->clients[i] && this->clients[i]->getNickName() == nickname)
            return (true);
    }
    return (false);
}

Client *Server::findClient(std::string const &name)
{
    for (size_t i = 0; i < this->maxClients; i++)
    {
        if (this->clients[i] && this->clients[i]->getNickName() == name)
            return (this->clients[i]);
    }
    return (NULL);
}

Client *Server::findClient(int fd)
{
    for (size_t i = 0; i < this->maxClients; i++)
    {
        if (this->clients[i] && this->clients[i]->getFd() == fd)
            return (this->clients[i]);
    }
    return (NULL);
}

void    Server::createChannel(Client const &client, std::string const &name, std::string const &key)
{
    this->channels[name] = new Channel(name);
    this->channels[name]->addOperator(client.getNickName());
    if (key != "")
    {
        this->channels[name]->setKey(key);
        this->channels[name]->setKMode(true);
    }
}
