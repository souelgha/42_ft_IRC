/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/24 12:37:33 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Server.hpp"

static int  getListeningPort(char *arg) {

    int         listening_port;

    for (size_t i = 0; arg[i]; i++)
    {
        if (!isdigit(arg[i]))
            throw (std::runtime_error("Listening port must be an integer between 6660 and 6669"));
    }
    listening_port = std::atoi(arg);
    if (listening_port < 6660 || listening_port > 6669)
        throw (std::runtime_error("Listening port must be an integer between 6660 and 6669"));
    return (listening_port);
}

static std::string const    getPassword(char *arg) {

    std::string const   password = arg;

    if (password.empty())
        throw (std::runtime_error("Password must be a simple word with printable character"));
    for (size_t i = 0; password[i]; i++)
    {
        if (!isprint(password[i]) || isspace(password[i]))
            throw (std::runtime_error("Password must be a simple word with printable character"));
    }
    return (password);
}

int main(int argc, char **argv) {


    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    try {
        int                 listening_port = getListeningPort(argv[1]);
        std::string const   password = getPassword(argv[2]);
        Server              server(listening_port, password);

        signal(SIGINT, Server::signalCatch);
        signal(SIGQUIT, Server::signalCatch);
        server.serverInit();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;    
    }
    std::cout
        << RED << "Server Closed" << WHITE << std::endl;
    return(0);
}