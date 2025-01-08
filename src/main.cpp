/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/08 17:02:49 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

static void checkPort(char const *argument, int listening_port) {

    for (size_t i = 0; argument[i]; i++)
    {
        if (!isdigit(argument[i]))
            throw(std::runtime_error("Wrong argument\n"));
    }
    if (atoi(argument) != listening_port)
        throw(std::runtime_error("Wrong argument\n"));
}

int main(int argc, char **argv) {

    Server server;

    if (argc != 2)  // a modifier: 2 => 3
    {
        std::cerr << "Usage: ./ircserv <port>" /* <password>" */ << std::endl;
        return (1);
    }
    try {
        checkPort(argv[1], server.getListenPort());
        signal(SIGINT, Server::SignalCatch);
        signal(SIGQUIT, Server::SignalCatch);
        server.serverInit();
    }
    catch(const std::exception& e) {
        server.closeFds();
        std::cerr << e.what() << std::endl;    
    }
    return(0);
}