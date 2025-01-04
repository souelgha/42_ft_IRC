
#include "Server.hpp"


Server::Server()
{
    listen_fd = -1;
}
Server::~Server(){}

void Server::ServerInit()
{
    this->port = 5094;
    Serverconnect();
    
    std::cout << "En attente de nouvelles connexions..." << std::endl;
    while (1)
    {
        if (poll(&fds[0], fds.size(), -1) == -1)
            throw(std::runtime_error("Failed to bind socket"));  
        
        for (size_t i = 0; i < fds.size(); i++)
        {
            std::cout<< "i de for :"<<i<< std::endl;
            if (fds[i].revents & POLLIN)
            {
                if(fds[i].fd == listen_fd)   // Accept New Client
                {
                    std::cout<< "New Client "<< std::endl;
                    NewClient();
                
                }
                 else
                {
                    std::cout<< "received data"<< std::endl;
                    ReceiveMessage(fds[i].fd);
                }
            }
        }
        std::cout<< "nfds:"<<fds.size()<< std::endl;
       
    }    
}

void Server::Serverconnect()
{
    // struct pollfd   Newpoll;
    // struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(this->port);
    sockAddr.sin_addr.s_addr = INADDR_ANY;

   
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
        throw(std::runtime_error("Echec Init du socket"));
   
    int sockAdLenght = sizeof(sockAddr);
    if(bind(listen_fd, (struct sockaddr*) &sockAddr, sockAdLenght) == -1);
        throw(std::runtime_error("Failed to bind socket"));     
    if(listen(listen_fd, MAX_CLIENTS) == -1)
        throw(std::runtime_error("Failed to listen()"));    
     
    Newpoll.fd = listen_fd;
    Newpoll.events = POLLIN;
    Newpoll.revents = 0;
    fds.push_back(Newpoll);

}
void Server::NewClient()
{
    // struct sockaddr_in CliAddr;
    // struct pollfd   Newpoll;
    int CliAddrLength=sizeof(CliAddr);
    int conn_fd = accept(listen_fd, (struct sockaddr*) &CliAddr, (socklen_t *) &CliAddrLength);
    if(conn_fd ==-1)
        throw(std::runtime_error("Failed to accept")); 

    if (fds.size() < MAX_CLIENTS)
    {
        std::cout<<"nb connections :"<< fds.size()<<std::endl;
        Newpoll.fd = conn_fd;
        Newpoll.events = POLLIN;
        Newpoll.revents = 0;
        fds.push_back(Newpoll);
        std::cout << "Connexion acceptée : adresse IP <" << inet_ntoa(CliAddr.sin_addr)<< "> port <" <<
           ntohs(CliAddr.sin_port) << ">"<<std::endl;
    }
    else
    {
        std::cerr << "Trop de clients"<< std::endl;
        close(conn_fd);
    }
}

void Server::ReceiveMessage(int fd)
{
    
    char buffer[BUFFER_SIZE] = {0};
    int receivedBytes = recv(fd, buffer, BUFFER_SIZE, 0);
    if(receivedBytes ==-1)
    {
        std::cerr << "<server> Echec de la reception du message du client "<< std::endl;
        //detruire le client . clearclient
        close (fd);
    }
    else
    {
        buffer[receivedBytes] = '\0';
        std::cout<<"IRSSI <"<<fd<< "> message: " << buffer<< std::endl;
    }

}
void Server::CloseFds()
{
    for(size_t i = 1; i< fds.size() ; i++)
        close(fds[i].fd);
    if(listen_fd != -1)
        close(listen_fd);
}


