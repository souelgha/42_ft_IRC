#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include <poll.h>

#define LISTENING_PORT 5094
#define PENDING_QUEUE_MAXLENGTH 10
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

int main()
{
   //initialisation du socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd==-1)
    {
        std::cerr << "Echec Init du socket "<< std::endl;
        return(1);
    }

    //configuration du socket
    struct sockaddr_in socketAdress;
    socketAdress.sin_family = AF_INET;
    socketAdress.sin_port = htons(LISTENING_PORT);
    socketAdress.sin_addr.s_addr = INADDR_ANY;

    //etablir la connection a l aide du bind()
    int socketAdressLenght = sizeof(socketAdress);
    int bindReturnCode = bind(listen_fd, (struct sockaddr*) &socketAdress, socketAdressLenght);
    if(bindReturnCode==-1)
    {
        std::cerr << "Echec de liaison pour le socket/ failed to bind socket "<< std::endl;
        return(1);
    }
    // on ecoute et attente de nouvelles connexions
    if(listen(listen_fd, PENDING_QUEUE_MAXLENGTH) == -1)
     {
        std::cerr << "Echec demarrage de l ecoute des connexions "<< std::endl;
        return(1);
    }
    std::cout << "En attente de nouvelles connexions..." << std::endl;
    /*************** boucle while a partir d ici  **************/

    struct pollfd   fds[MAX_CLIENTS];
    int             nfds = 1;
    int             conn_fd;

    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    while (1)
    {
        int ret = poll(fds, nfds, -1);
        if (ret < 0)
        {
            std::cerr << "Poll erreur"<< std::endl;
            break ;
        }
        for (int i = 0; i < nfds; i++)
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
                    if (nfds < MAX_CLIENTS)
                    {
                        std::cout<<"nfds connect:"<< nfds<<std::endl;
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
        std::cout<< "nfds:"<<nfds<< std::endl;
       
    }
    // on cree un autre socket pour nous connecter

    // reception d un message /* on a besoin d une memoire tampon et conversion en bytes*/
    //Fermeture des sockets et liberation des ressources
    close(conn_fd);
    close(listen_fd);
    return(0);
}

