/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:26:51 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/06 15:50:41 by stouitou         ###   ########.fr       */
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
    while (Server::signal== false)//signaux a ajouter
    {
        std::cout<<  "signal :"<< Server::signal <<std::endl;      
        if (poll(&fds[0], fds.size(), -1) == -1 )
            throw(std::runtime_error("Failed to bind pool\n"));  
        
        for (size_t i = 0; i < fds.size(); i++)
        {
            // std::cout<< "i de for :"<<i<< std::endl;
            if (fds[i].revents & POLLIN)
            {
                if(fds[i].fd == listen_fd)   // Accept New Client
                {
                    std::cout<< BLUE << "New Client "<< WHITE<< std::endl;
                    NewClient();
                
                }
                 else
                {
                    std::cout<<YELLOW<< "received data" << WHITE<<std::endl;
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
    sockAddr.sin_family = AF_INET;          // pour IPV4
    sockAddr.sin_port = htons(this->port);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t sockAdLenght = sizeof(sockAddr);
   
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
        throw(std::runtime_error("Echec Init du socket\n"));
   
   int en = 1;
	if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket\n"));
	
    if(bind(listen_fd, (struct sockaddr*) &sockAddr, sockAdLenght) == -1)
        throw(std::runtime_error("Failed to bind socket " + std::string(strerror(errno))));     
    if(listen(listen_fd, MAX_CLIENTS) == -1)
        throw(std::runtime_error("Failed to listen()\n"));    
     
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

void commandNick(std::string const &message, Client &client) {

    std::string nickname;

    nickname = message.substr(5, message.length() - 4);
    std::cout << "NickName: <" << nickname << ">" << std::endl;
    try {
        client.setNickName(nickname);
    }
    catch (std::exception &e) {
        throw ;
    }
}

std::string getRealName(std::string message) {

    size_t i = message.find(':');

    // if (i == std::string::npos)
    //     throw Error;
    return (message.substr(i + 1, message.length() - i));
}

void commandUser(std::string const &message, Client &client) {

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
        std::cout << "Server name: <" << serverName << ">" << std::endl;
       realName = getRealName(message);
        std::cout << "Real name: <" << realName << ">" << std::endl;
        client.setRealName(realName);
    }
    catch (std::exception &e) {
        throw;
    }
}

std::string getCommand(std::string message) {

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
        }
        catch (std::exception &e) {
            throw;
        }
        start = end + 2; // 2 est la taille du delimiteur
    }
}

void    sendAnswer(Client const &client) {

    const char msg[] = ":127.0.0.1 001 stouitou :Welcome to the IRC Network stouitou!stouitou@stouitou\r\n";
    int sentBytes = send(client.getFd(), msg, strlen(msg), 0);
    if (sentBytes == -1)
    {
        std::cerr << "<server> Echec envoi du message pour le client " << std::endl;
        // throw ;
    }
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
    signal = true;
    throw(std::runtime_error("Signal recu "));

}
Channel * Server::createChannel(std::string const &name)
{       
    if(channels.find(name) == channels.end())
    {
        channels[name] = Channel(name);
    }
    return(&Channel(name));
}
void Server::deleteChannel(std::string const &name)
{
    if(channels.find(name) != channels.end())
        channels.erase(name);
}
Channel * Server::getChannel(std::string const &name)
{
    if(channels.find(name) != channels.end())
        return(&Channel(name));
    return(NULL);

}
 std::vector<std::string> Server::followlistChannels() 
 {
    std::map<std::string, Channel>::const_iterator it;
    for(it =  channels.begin(); it != channels.end(); ++it)
        listChannel.push_back(it->first);
    return(listChannel);
 }
 void Server::HandleJoinCommand(std::string nickname, std::string channelname)
 {
    Channel *channel = getChannel(channelname);
    if(!channel)
    {
        channel = createChannel(channelname);
        channel->AddOper(nickname);
    }
    channel->AddUser(nickname);
    std::string welcomeMessage = ":" + nickname + " JOIN " + channelname + "\r\n";
    sendToClient(nickname, welcomeMessage);
 }
 void Server::sendToClient(std::string const nickname, std::string const message)
 {
    for(size_t i = 0; i < clients.size(); i++)
    {
        if(clients[i].getNickName() == nickname)
        {  
            send(clients[i].getFd(), message.c_str(), message.size(), 0);            
        }
        else
            throw(std::runtime_error("utilisateur introuvable\n"));

    }

 }
