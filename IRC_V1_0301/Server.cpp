
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
                    conn_fd = accept(listen_fd, (struct sockaddr*) &socketAdress, (socklen_t *) &socketAdressLenght);
                    if(conn_fd ==-1)
                    {
                        std::cerr << "<server> Echec etablissement de la connection "<< std::endl;
                        continue ;
                    }                    
                    if (fds.size() < MAX_CLIENTS)
                    {
                        std::cout<<"nfds connect:"<< fds.size()<<std::endl;
                        fds[nfds].fd = conn_fd;
                        fds[nfds].events = POLLIN;
                        nfds++;
                    }
                    else
                    {
                        std::cerr << "Trop de clients"<< std::endl;
                        close(conn_fd) ;
                    }
                }
                 else //receive new data
                {
                    std::cout<< "received data"<< std::endl;
                    char buffer[BUFFER_SIZE] = {0};
                    int receivedBytes = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
                    if(receivedBytes ==-1)
                    {
                        std::cerr << "<server> Echec de la reception du message du client "<< std::endl;
                        close (fds[i].fd);
                        fds[i] = fds[--nfds];
                    }
                    else
                    {
                        // buffer[receivedBytes] = '\0';
                        std::cout<<"IRSSI : "<< buffer<< std::endl;
                    }
                }
            }
        }
        std::cout<< "nfds:"<<fds.size()<< std::endl;
       
    }
    
}

void Server::Serverconnect()
{
    struct pollfd   Newpoll;
    struct sockaddr_in sockAddr;
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
int main()
{
  
    /*************** boucle while a partir d ici  **************/

       int             nfds = 1;
    int             conn_fd;

    
               
    // on cree un autre socket pour nous connecter

    // reception d un message /* on a besoin d une memoire tampon et conversion en bytes*/
    //Fermeture des sockets et liberation des ressources
    close(conn_fd);
    close(listen_fd);
    return(0);
}

