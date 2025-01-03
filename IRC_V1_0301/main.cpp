
#include "Server.hpp"


int main()
{
   Server serv;

   serv.ServerInit();
    /*************** boucle while a partir d ici  **************/

    
    int             nfds = 1;
    int             conn_fd;

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

    close(conn_fd);
    close(listen_fd);
    return(0);
}

