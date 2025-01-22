#include "Server.hpp"

bool    Server::signal = false;

Server::Server(void) { }

Server::Server(int listening_port, std::string const &password) : name("ourIRC"), password(password), listen_port(listening_port), listen_fd(-1) { }

Server::~Server(void) {

    this->closeFds();
    this->clearChannels();
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

std::vector<Client> &Server::getClients(void) {

    return (this->clients);
}

std::map<std::string, Channel>  &Server::getChannels(void) {

    return (this->channels);
}

void    Server::serverInit(void)
{
    try {
        serverConnect();
    }
    catch (std::exception &e) {
        throw ;
    }

    std::cout
        << RED << "En attente de nouvelles connexions..." << WHITE << std::endl;
    while (Server::signal == false)
    {
        if (poll(&this->fds[0], this->fds.size(), -1) == -1)
            throw(std::runtime_error("Failed to bind poll\n"));
        for (size_t i = 0; i < this->fds.size(); i++)
        {
            if (this->fds[i].revents & POLLIN)    // si un evenement a eu lieu
            {
                try {
                    if (this->fds[i].fd == this->listen_fd)  // si cet evenement a lieu sur le port d'ecoute => ajouter un nouveau client
                        newClient();
                    else
                        receiveMessage(this->clients[i - 1]);
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
    if(this->listen_fd == -1)
        throw(std::runtime_error("Failed to initialise socket"));

    int en = 1;
	if(setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("Failed to set option on socket"));
    if (fcntl(this->listen_fd, F_SETFL, O_NONBLOCK) == -1) 
		throw(std::runtime_error("Failed to set option on socket"));

    // lie le socket a l'adresse
    if(bind(this->listen_fd, (struct sockaddr*) &this->listenAddress, listenAddressLength) == -1)
        throw(std::runtime_error("Failed to bind socket"));

    // commence a ecouter
    if(listen(this->listen_fd, MAX_CLIENTS) == -1)
        throw(std::runtime_error("Failed to listen"));    

    // permet de verifier les evenements
    struct pollfd   newPoll;
    newPoll.fd = listen_fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->fds.push_back(newPoll);
}

void Server::newClient(void)
{
    std::cout
        << BLUE << "New Client" << WHITE << std::endl;

    Client newClient;

    int clientAddressLength = sizeof(this->clientAddress);
    int connection_fd = accept(this->listen_fd, (struct sockaddr*) &this->clientAddress, (socklen_t *) &clientAddressLength);
    if (connection_fd == -1)
        throw(std::runtime_error("Failed to accept")); 
    if (this->fds.size() < MAX_CLIENTS)
    {
        struct pollfd   newPoll;
        newPoll.fd = connection_fd;
        newPoll.events = POLLIN;
        newPoll.revents = 0;
        this->fds.push_back(newPoll);
        newClient.setFd(connection_fd);
        newClient.setIpAddress(inet_ntoa(this->clientAddress.sin_addr));
        this->clients.push_back(newClient);
    }
    else
    {
        std::cerr
            << "Too many clients" << std::endl;
        close(connection_fd);
    }
}

static bool incompleteCommand(char *buffer) {

    if (strstr(buffer, DELIMITER))
        return (false);
    return (true);
}

void Server::receiveMessage(Client &client)
{
    std::cout
        << YELLOW << "Message received from " << client.getNickName() << " (" << client.getFd() << "):" << WHITE << std::endl;

    size_t  buffer_len = std::strlen(client.buffer);
    int     receivedBytes = recv(client.getFd(), client.buffer + buffer_len, BUFFER_SIZE - buffer_len, 0);
    if (receivedBytes <= 0)
    {
        std::cerr
            << "Failed to receive client's message" << std::endl;
        clearClient(client.getFd());
        return ;
    }
    std::cout<<MAGENTA<< "client.buffer_avantinco: " << client.buffer<< WHITE<< std::endl;
    if (incompleteCommand(client.buffer))
        return ;
    client.buffer[receivedBytes+ buffer_len] = '\0';
    std::cout<<MAGENTA<< "client.buffer " << client.buffer<< WHITE<< std::endl;
    client.commandReact(*this);
}

void Server::closeFds(void)
{
    for(size_t i = 1; i< fds.size() ; i++)
        close(fds[i].fd);
    if(listen_fd != -1)
        close(listen_fd);
}

void Server::clearClient(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getFd() == fd)
        {
            close(clients[i].getFd());
            clients.erase(clients.begin() + i);
            break;
        }
    }
    for(size_t i = 0; i< fds.size(); i++)
    {
        if(fds[i].fd == fd)
        {
            fds.erase(fds.begin() + i);
            break;
        }
    }
}

void Server::clearbuffer(char *buffer)
{
    std::fill(buffer, buffer + BUFFER_SIZE, 0);
}

void Server::signalCatch(int)
{
    signal = true;
    throw(std::runtime_error(""));
}

void    Server::replyUser(Client const &client) {

    std::string const message = RPL_WELCOME(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}

Client &Server::findClient(std::string const &name)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getNickName() == name)
            return (this->clients[i]);
    }
    throw (std::runtime_error("Nickname does not exist"));
}

bool    Server::isClient(std::string const &nickname) {

    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getNickName() == nickname)
            return (true);
    }
    return (false);
}

void    Server::createChannel(Client const &client, std::string const &name, std::string const &key)
{
    this->channels[name] = Channel(name);
    this->channels[name].addOper(client.getNickName());
    if(key != "")
    {
        this->channels[name].setKey(key);
        this->channels[name].setKMode(true);
    }
}

void    Server::deleteChannel(std::string const &name)
{
    if (channels.find(name) != channels.end())
        channels.erase(name);
}

void    Server::clearChannels(void) {

    this->channels.clear();
}
