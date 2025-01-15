/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/14 14:11:35 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

static void checkArgs(char **argv, int listening_port) {

    for (size_t i = 0; argv[1][i]; i++)
    {
        if (!isdigit(argv[1][i]))
            throw(std::runtime_error("Wrong port\n"));
    }
    if (atoi(argv[1]) != listening_port)
        throw(std::runtime_error("Wrong port\n"));
    //ajout du check du password et enregistrement du pwd
    if(strcmp(argv[2], PASSWORD) != 0)
        throw(std::runtime_error("Wrong password\n"));
}

int main(int argc, char **argv) {

    Server server;

    if (argc != 3)  // a modifier: 2 => 3
    {
        std::cerr << "Usage: ./ircserv <port> <password>" /* <password>" */ << std::endl;
        return (1);
    }
    try {
        checkArgs(argv, server.getListenPort());
        signal(SIGINT, Server::signalCatch);
        signal(SIGQUIT, Server::signalCatch);
        server.serverInit();
    }
    catch(const std::exception& e) {
        server.closeFds();
        std::cerr << e.what() << std::endl;    
    }
    std::cout<<"Server Closed"<< std::endl;
    return(0);
}