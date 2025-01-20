/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/20 11:40:36 by stouitou         ###   ########.fr       */
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

    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    try {
        checkArgs(argv, server.getListenPort());
        signal(SIGINT, Server::signalCatch);
        signal(SIGQUIT, Server::signalCatch);
        server.serverInit();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;    
    }
    server.closeFds();
    server.clearChannels();
    std::cout << "Server Closed" << std::endl;
    return(0);
}