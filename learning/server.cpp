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
        if (fds[0].revents & POLLIN)
        {
            conn_fd = accept(listen_fd, (struct sockaddr*) &socketAdress, (socklen_t *) &socketAdressLenght);
            if(conn_fd ==-1)
            {
                std::cerr << "<server> Echec etablissement de la connection "<< std::endl;
                continue ;
            }
            std::cout<< "on est la "<< std::endl;
            if (nfds < MAX_CLIENTS)
            {
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
        std::cout<< "nfds:"<<nfds<< std::endl;
        for (int i = 1; i < nfds; i++)
        {
            std::cout<< "on est dans le for"<< std::endl;
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
                std::cout<<"client: "<< buffer<< std::endl;
            }
            //envoie d un message
            // const char msg[]="Bonjour Client, je suis server. ";
            // int sentBytes = send(conn_fd, msg, strlen(msg), 0);
            // if(sentBytes ==-1)
            // {
            //     std::cerr << "<server> Echec envoi du message pour le client "<< std::endl;
            //     return(1);
            // }
        std::cout<< "on sort du for"<< std::endl;
        std::cout<< "i de for:"<<i<< std::endl;

        }
        std::cout<< "apres for"<< std::endl;

    }
    // on cree un autre socket pour nous connecter

    // reception d un message /* on a besoin d une memoire tampon et conversion en bytes*/
    //Fermeture des sockets et liberation des ressources
    close(conn_fd);
    close(listen_fd);


    return(0);
}

/* 1_____ initialisation du socket
 int socket(int domain, int type, int protocol)
    AF_INET pour IPv4
    SOCK_STREAM pour etre en TCP
    protocol => on met ) car on a rien de specifique a lancer.
    SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
    en cas d erreur les fonctions return (-1)

*/
/* 2______ structure pour configurer le socket 
struct sockaddr_in {
 sa_family_t     sin_family; //AF_INET
 in_port_t       sin_port;// Port number 
 struct  in_addr sin_addr; // IPv4 address
//  char            sin_zero[8];
};
*/

// but du server == ecouter sur un numero de port

/* 3_______ pour etablir la connection on fait un bind() 
on fait la liaison entre les infos mises et un socket
bind(socket, pointeur sur la struc de sockaddr, la taille de l adress)*/

/* 4________ mettre le server sur ecoute 
if(listen())
backlog => file d attente. permet de limiter le nombre de client en file d attente */

/* 5________ accepter les connexions entrantes sur un socket
on cree un autre socket qui va nous permettre de nous connecter */