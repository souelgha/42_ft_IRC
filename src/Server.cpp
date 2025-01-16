#include "Server.hpp"

bool    Server::signal = false;

Server::Server(void) : listen_port(LISTENING_PORT), listen_fd(-1) { }

Server::~Server(void) { }

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
    serverConnect();

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
                if (this->fds[i].fd == this->listen_fd)  // si cet evenement a lieu sur le port d'ecoute => ajouter un nouveau client
                    newClient();
                else
                    receiveMessage(this->clients[i - 1]);
            }
        }
    }   
    closeFds();
}

void    Server::serverConnect(void)
{
    this->listenAddress.sin_family = AF_INET;                     
    this->listenAddress.sin_port = htons(this->listen_port);      
    this->listenAddress.sin_addr.s_addr = INADDR_ANY;             
    socklen_t listenAddressLength = sizeof(this->listenAddress);  

    this->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->listen_fd == -1)
        throw(std::runtime_error("Failed to initialise socket\n"));

    int en = 1;
	if(setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("Failed to set option on socket\n"));

    // lie le socket a l'adresse
    if(bind(this->listen_fd, (struct sockaddr*) &this->listenAddress, listenAddressLength) == -1)
        throw(std::runtime_error("Failed to bind socket " + std::string(strerror(errno)))); // supprimer errno

    // commence a ecouter
    if(listen(this->listen_fd, MAX_CLIENTS) == -1)
        throw(std::runtime_error("Failed to listen\n"));    

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
        << BLUE << "New Client " << WHITE << std::endl;

    Client newClient;

    int clientAddressLength = sizeof(this->clientAddress);
    int connection_fd = accept(this->listen_fd, (struct sockaddr*) &this->clientAddress, (socklen_t *) &clientAddressLength);
    if(connection_fd == -1)
        throw(std::runtime_error("Failed to accept\n")); 
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

void Server::receiveMessage(Client &client)
{
    std::cout
        << YELLOW << "Message received from " << client.getNickName() << " (" << client.getFd() << "):" << WHITE << std::endl;

    int     receivedBytes = recv(client.getFd(), client.buffer, BUFFER_SIZE, 0);
    if (receivedBytes == -1)
    {
        std::cerr
            << "Failed to receive client's message" << std::endl;
        clearClient(client.getFd());   // supprime le client
        return ;
    }
    client.buffer[receivedBytes] = '\0';
    if(!client.commandConnect(*this))
        clearClient(client.getFd());
    else
        client.commandReact(*this);
        
}

void Server::closeFds(void)
{
    for(size_t i = 1; i< fds.size() ; i++)
        close(fds[i].fd);
    if(listen_fd != -1)
        close(listen_fd);
}

void Server::clearClient(int fd) // retrait du vector client & du vector sockets.
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
    throw (std::runtime_error("Nickname does not exist\n"));
}

void    Server::replyNick(Client &client, std::string const &newnick) {
    std::string const message = ":"+client.getSourceName()+  " NICK :"+newnick+ CRLF;
    sendTemplate(client, message);
}
void    Server::replyErrNick(Client &client) {
    
    std::string const message = ERR_NICKNAMEINUSE(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}
void    Server::replyErronNickUse(Client &client) {
    
    std::string const message = ERR_ERRONEUSNICKNAME(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}


void    Server::replyModeClient(Client const &client) {

    std::string const message = RPL_UMODEIS(client.getServerName(), client.getNickName(), client.getMode());
    sendTemplate(client, message);
}

void    Server::replyModeChannel(Client const &client, Channel &channel, std::string const &mode) {

    std::string message = "";

    if (!mode.empty())
    {
        channel.setMode(mode);
        message = RPL_CHANNELMODEIS1(client.getSourceName(), channel.getName(), channel.convertMode());
    }
    else
        message = RPL_CHANNELMODEIS2(client.getServerName(), client.getNickName(), channel.getName(), channel.convertMode());

    sendTemplate(client, message);
}

void    Server::replyQuit(Client &client, std::string const &reason) {

    std::string message = ":"
            + client.getServerName()
            + " QUIT";

    if (!reason.empty())
        message += ": " + reason;
    message += CRLF;

    sendTemplate(client, message);
    clearClient(client.getFd());
}

void    Server::replyJoin(Client const &client, Channel &channel) {

    // message de bienvenue dans le canal
    {
        std::string message = ":" + client.getNickName() + " JOIN :" + channel.getName() + "\r\n";

        std::cout << GREEN << ">> " << message << WHITE << std::flush;
        for (size_t i = 0; i < channel.getUsers().size(); i++)
            sendTemplate(channel.getUsers()[i], message);
    }

    // Le serveur envoie ensuite une liste des utilisateurs presents dans le canal. Le format est:
    //     :<servername> 353 <nickname> = <channel> :<users>
    //     353: code numerique pour RPL_NAMREPLY
    //     <nickname>: nom de l'utilisateur qui a rejoint le canal
    //     <channel>: nom du canal
    //     <users>: liste des utilisateurs dans le canal, separes par des espace. Les prefixes (@, +) indiquent les privileges (operateurs, ...)
    {
        std::string message = RPL_NAMREPLY(client.getServerName(), client.getNickName(), channel.getName());

        for (size_t i = 0; i < channel.getUsers().size(); i++)
        {
            if (channel.isOperator(channel.getUsers()[i].getNickName()))
                message += "@";
            message += channel.getUsers()[i].getNickName() + " ";
        }
        message += CRLF;
        sendTemplate(client, message);

    }

    // Le serveur termine avec un message:
    //     :<servername> 366 <nickname> <channel> :End of /NAMES list
    //     366: code numerique pour RPL_ENDOFNAMES
    //     <nickname>: nom de l'utilisateur qui a rejoint le canal
    //     <channel>: nom du canal
    // :<server> 366 <nickname> <channel> :End of /NAMES list.
    {
        std::string message = RPL_ENDOFNAMES(client.getServerName(), client.getNickName(), channel.getName());
        sendTemplate(client, message);
    }

    // Le serveur peut envoyer un message facultatif RPL_TOPIC
    //     :<servername> 332 <nickname> <channel> :<topic>
    // {
    //     std::string message = client.getNickName() + " has join " + channel.getName() + "\r\n";

    //     std::cout << GREEN << ">> " << message << WHITE << std::flush;
    //     int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    //     if (sentBytes == -1)
    //         throw(std::runtime_error("Failed to send message to client\n")) ;
    // }
}

void    Server::replyPart(Client const &client, Channel &channel) {

    std::string message = ":" + client.getSourceName() + " PART " + channel.getName() + "\r\n";

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
        sendTemplate(channel.getUsers()[i], message);
    channel.RemUser(client);
    channel.RemOper(client.getNickName());
}

void    Server::replyPrivmsgClient(Client const &sender, Client const &recipient, std::string const &toSend) {

    std::string message = ":" + sender.getNickName() + " PRIVMSG " + recipient.getNickName() + " " + toSend + "\r\n";

    sendTemplate(recipient, message);
}

void    Server::replyPrivmsgChannel(Client const &sender, Channel &channel, std::string const &toSend) {

    std::string message = ":" + sender.getNickName() + " PRIVMSG " + channel.getName() + " :" + toSend + "\r\n";

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        if (channel.getUsers()[i].getNickName() != sender.getNickName())
            sendTemplate(channel.getUsers()[i], message);
    }
}

void    Server::replyTopic(Client const &client, Channel &channel, std::string const &topic) {

    std::string message = RPL_TOPIC(client.getServerName(), client.getNickName(), channel.getName(), topic);

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
        sendTemplate(channel.getUsers()[i], message);
}

void    Server::replyKick(Client const &client, Channel &channel, Client const &recipient, std::string const &reason) {

    std::string message = RPL_KICK(client.getSourceName(), channel.getName(), recipient.getNickName(), reason);

    std::cout << GREEN << ">> " << message << WHITE << std::flush;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        sendTemplate(channel.getUsers()[i], message);
    }
    channel.RemUser(recipient);
    channel.RemOper(recipient.getNickName());
}

void    Server::replyInvite(Client const &sender, Client const &recipient, Channel &channel) {

    std::string message = RPL_INVITING(sender.getServerName(), sender.getNickName(), recipient.getNickName(), channel.getName());
    sendTemplate(sender, message);
    sendTemplate(recipient, message);
}

void    Server::replyWho(Client const &client, Channel &channel) 
{
    // std::cout << "ici. channel size:" <<  channel.getUsers().size()<< std::endl;
    for (size_t i = 0; i < channel.getUsers().size(); i++)
    {
        std::string message1 = RPL_WHOREPLY(client.getServerName(), client.getNickName(), 
                                channel.getName(), channel.getUsers()[i].getNickName(), 
                                channel.getUsers()[i].getUserName(), channel.getUsers()[i].getHostName(), 
                                channel.getUsers()[i].getRealName());
        // std::cout << "message who:<"<<message1<<">"<< std::endl;
        sendTemplate(client, message1);
    }
    std::string message2 = RPL_ENDOFWHO(client.getServerName(), client.getNickName(), channel.getName());
    sendTemplate(client, message2);

}

void    Server::replyPing(Client const &client, std::string const &pong) {

    std::string message = ":"
            + client.getServerName() + " PONG :" 
            + pong + CRLF;
    sendTemplate(client, message);
}


void    Server::replyUnknown(Client const &client, std::string const &command) {

    std::string message = "Unknown command " + command + CRLF;
    sendTemplate(client, message);
}

Channel &Server::findChannel(Client const &client, std::string const &name)
{
    if (channels.find(name) == channels.end())
    {
        channels[name] = Channel(name);
        channels[name].AddOper(client.getNickName());
    }
    channels[name].AddUser(client);
    Channel &channel = channels[name];
    return(channel);
}

void    Server::deleteChannel(std::string const &name)
{
    if (channels.find(name) != channels.end())
        channels.erase(name);
}
