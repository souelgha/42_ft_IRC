/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stouitou <stouitou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:45:23 by stouitou          #+#    #+#             */
/*   Updated: 2025/01/07 15:50:24 by stouitou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

int main(void)
{
    Server server;
    
    try {
        signal(SIGINT, Server::SignalCatch);
        signal(SIGQUIT, Server::SignalCatch);
        server.serverInit();
    }
    catch(const std::exception& e) {
        server.closeFds();
        std::cerr << e.what() << std::endl;    
    }
    std::cout<<"Server Closed"<< std::endl;
    return(0);
}