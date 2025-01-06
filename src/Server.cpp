
#include "Server.hpp"


Server::Server()
{
    listen_fd = -1;
    
}
Server::~Server(){}
bool Server::signal = false;
void Server::ServerInit()
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
                    std::cout<<YELLOW<< "received data"<< WHITE<<std::endl;
                    ReceiveMessage(clients[i]);
                }
            }
        }
        std::cout<< "nombre de fds:"<<fds.size()<< std::endl;
       
    }   
    std::cout<<" sortie du while" << std::endl;
    CloseFds();
}

void Server::Serverconnect()
{
    sockAddr.sin_family = AF_INET;
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

void Server::ReceiveMessage(Client cli)
{
    
    char buffer[BUFFER_SIZE] = {0};
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
        std::cout << YELLOW <<"IRSSI avec fd <"<<fd<< "> envoie le message: " << buffer<< YELLOW<< std::endl;
    }

}
void Server::handleReceivedMessage(char *buff, Client client) {

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


