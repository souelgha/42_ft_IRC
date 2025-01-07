/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:26:51 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/07 11:59:15 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::signal = false;

Server::Server(void) : listen_fd(-1) { }

Server::~Server(void) { }

void    Server::ServerInit(void)
{
    this->port = 5095;
    Serverconnect();

    std::cout << "En attente de nouvelles connexions..." << std::endl;
    while (Server::signal == false)
    {
        if (poll(&fds[0], fds.size(), -1) == -1)
            throw(std::runtime_error("Failed to bind pool\n"));  
        
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if(fds[i].fd == listen_fd)  // port d'ecoute => accepte un nouveau client
                {
                    std::cout << BLUE << "New Client " << WHITE << std::endl;
                    NewClient();
                }
                else
                {
                    std::cout << YELLOW << "Received data" << WHITE << std::endl;
                    ReceiveMessage(clients[i - 1]);
                }
            }
        }
        std::cout<< "nombre de fds:"<<fds.size()<< std::endl;
    }   
    std::cout<<" sortie du while" << std::endl;
    CloseFds();
}

void    Server::Serverconnect(void)
{
    sockAddr.sin_family = AF_INET;              // pour IPV4
    sockAddr.sin_port = htons(this->port);      // convertit le port dans le bon format
    sockAddr.sin_addr.s_addr = INADDR_ANY;      // ecoute tous les ports
    socklen_t sockAdLenght = sizeof(sockAddr);  // taille de l'adresse

    // ouvre la liaison
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
        throw(std::runtime_error("Echec Init du socket\n"));

    // initialise les options (SO_REUSEADDR => pour reutiliser l'adresse)
    int en = 1;
	if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket\n"));

    // lie le socket a l'adresse
    if(bind(listen_fd, (struct sockaddr*) &sockAddr, sockAdLenght) == -1)
        throw(std::runtime_error("Failed to bind socket " + std::string(strerror(errno))));     

    // commence a ecouter
    if(listen(listen_fd, MAX_CLIENTS) == -1)
        throw(std::runtime_error("Failed to listen()\n"));    

    // Pour verifier les evenements
    Newpoll.fd = listen_fd;
    Newpoll.events = POLLIN;
    Newpoll.revents = 0;
    fds.push_back(Newpoll);
}
void Server::NewClient()
{

    Client Newcli;
    int CliAddrLength=sizeof(CliAddr);
    int conn_fd = accept(listen_fd, (struct sockaddr*) &CliAddr, (socklen_t *) &CliAddrLength);
    if(conn_fd ==-1)
        throw(std::runtime_error("Failed to accept\n")); 

    if (fds.size() < MAX_CLIENTS)
    {
        std::cout<<"nb connections :"<< fds.size()<<std::endl;
        Newpoll.fd = conn_fd;
        Newpoll.events = POLLIN;
        Newpoll.revents = 0;
        fds.push_back(Newpoll);
        Newcli.setFd(conn_fd);
        Newcli.setIpAdd(inet_ntoa(CliAddr.sin_addr));
        clients.push_back(Newcli);
        std::cout << "Connexion acceptée : adresse IP <" << inet_ntoa(CliAddr.sin_addr)<< "> port <" <<
           ntohs(CliAddr.sin_port) << ">"<<std::endl;
    }
    else
    {
        std::cerr << "Trop de clients"<< std::endl;
        close(conn_fd);
    }
}

void Server::commandNick(std::string const &message, Client &client) {

    std::string nickname;

    nickname = message.substr(5, message.length() - 4);
    try {
        client.setNickName(nickname);
        std::cout << "After NICK command, client's nickname = <" << client.getNickName() << ">" << std::endl;
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::string Server::getRealName(std::string message) {

    size_t i = message.find(':');

    // if (i == std::string::npos)
    //     throw Error;
    return (message.substr(i + 1, message.length() - i));
}

void Server::commandUser(std::string const &message, Client &client) {

    std::string         userName;
    std::string         hostName;
    std::string         serverName;
    std::string         realName;
    std::istringstream  datas(message);

    std::string command;
    datas >> command;
    try {
        datas >> userName;
        client.setUserName(userName);
        datas >> hostName;
        client.setHostName(hostName);
        datas >> serverName;
        client.setServerName(serverName);
        realName = getRealName(message);
        client.setRealName(realName);
        std::cout << "After USER command, client's user name = <" << client.getUserName() << ">, host name = <" << client.getHostName() << ">, real name = <" << client.getRealName() << ">, server's name = <" << client.getServerName() << ">" << std::endl;
        sendAnswer("USER", client);
    }
    catch (std::exception &e) {
        throw;
    }
}

void    Server::commandMode(std::string const &message) {

    std::string         command;
    std::string         mode;
    std::string         nickname;
    std::istringstream  datas(message);
    size_t              i;

    datas >> command;
    datas >> nickname;
    datas >> mode;
    std::cout << "mode: <" << mode << ">" << std::endl;
    for (i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getNickName() == nickname)
            break ;
    }
    // if (i == this->clients.size())
    //     throw ;
    try {
        this->clients[i].setMode(mode);
    }
    catch (std::exception &e) {
        throw ;
    }
}

void    Server::commandWhois(std::string const &message) {

    std::string whois;
    size_t i;

    whois = message.substr(6, message.length() - 5);
    std::cout << "whois: <" << whois << ">" << std::endl;
    for (i = 0; i < this->clients.size(); i++)
    {
        std::cout << "i = " << i << ", nickname = " << this->clients[i].getNickName() << std::endl;
        if (this->clients[i].getNickName() == whois)
            break ;
    }
    // if (i == this->clients.size())
    //     throw ;
    try {
        answerWhois(this->clients[i]);
    }
    catch (std::exception &e) {
        std::cout << "Error in Whois" << std::endl;
        throw ;
    }
}

void    Server::commandPing(std::string const &message, Client &client) {

    std::string ping;

    ping = message.substr(5, message.length() - 4);
    std::cout << "ping: <" << ping << ">" << std::endl;
    try {
        answerPing(client, ping);
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::string Server::getCommand(std::string message) {

    size_t i = message.find(' ');

    if (i == std::string::npos)
        return (message);
    return (message.substr(0, i));
}

void Server::ReceiveMessage(Client &cli)
{
    
    char buffer[BUFFER_SIZE] = {0};
    std::cout << "fd client: " << cli.getFd() << std::endl;
    int receivedBytes = recv(cli.getFd(), buffer, BUFFER_SIZE, 0);
    if(receivedBytes ==-1)
    {
        std::cerr << "<server> Echec de la reception du message du client "<< std::endl;
        ClearClients(cli.getFd()); //detruire le client . clearclient
        close (cli.getFd());
    }
    else
    {
        buffer[receivedBytes] = '\0';
        handleReceivedMessage(buffer, cli);
        std::cout << YELLOW <<"IRSSI avec fd <"<< cli.getFd() << "> envoie le message: " << buffer<< WHITE << std::endl;
    }

}
void Server::handleReceivedMessage(char *buff, Client &client) {

    std::string buffer = buff;
    std::string message;
    std::string command;
    size_t start = 0;
    size_t end;

    while ((end = buffer.find(DELIMITER, start)) != std::string::npos)
    {
        message = buffer.substr(start, end - start);
        command = getCommand(message);
        try {
            if (command == "NICK")
                commandNick(message, client);
            if (command == "USER")
                commandUser(message, client);
            if (command == "MODE")
                commandMode(message);
            if (command == "WHOIS")
                commandWhois(message);
            if (command == "PING")
                commandPing(message, client);
        }
        catch (std::exception &e) {
            throw;
        }
        start = end + 2; // 2 est la taille du delimiteur
    }
}

void    Server::answerUser(Client &client) {

    std::string message = ":";

    message += client.getServerName();
    message += " 001 ";
    message += client.getNickName();
    message += " :Welcome to the IRC Network ";
    message += client.getNickName();
    message += "!";
    message += client.getNickName();
    message += "@";
    message += client.getNickName();
    message += "\r\n";

    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
}

void    Server::answerMode(Client &client) {

    std::string message = ":";

    message += client.getServerName();
    message += " MODE ";
    message += client.getNickName();
    message += " :";
    message += client.getMode();
    message += "\r\n";

    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
}

void    Server::answerWhois(Client &client) {

    std::string message = ":";

    message += client.getServerName();
    message += " 311 ";
    message += client.getNickName();
    message += " ";
    message += client.getUserName();
    message += " ";
    message += client.getHostName();
    message += " * :";
    message += client.getRealName();
    message += "\r\n";

    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
}

void    Server::answerPing(Client &client, std::string const &ping) {

    std::string message = ":";

    message += client.getServerName();
    message += " PONG :";
    message += ping;
    message += "\r\n";

    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
}

void    Server::sendAnswer(std::string const &command, Client &client) {

    if (command == "USER")
        answerUser(client);
    if (command == "MODE")
        answerMode(client);
}

void Server::CloseFds()
{
    for(size_t i = 1; i< fds.size() ; i++)
    {
        close(fds[i].fd);
        std::cout<<"fd<" << fds[i].fd << "> closed "<< std::endl;
    }
    if(listen_fd != -1)
    {
        close(listen_fd);
        std::cout<<"listen_fd<" << listen_fd << "> closed "<< std::endl;
    }
}
void Server::ClearClients(int fd) //retrait du vector client & du vector sockets.
{
    for(size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getFd() == fd)
        {
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

void Server::SignalCatch(int signum)
{
    (void)signum;
    std::cout << "Signal recu! " << std::endl;
    signal = true;
    throw(std::runtime_error("Signal recu "));

}


