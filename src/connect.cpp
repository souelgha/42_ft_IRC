#include"Server.hpp"
#include"Client.hpp"

bool    Client::commandConnect(Server &server) 
{
    std::string buffer = this->buffer;
    std::size_t cap = buffer.find("CAP");
    std::size_t pwd = buffer.find("PASS");
    if(cap != std::string::npos)
    {        
        if (pwd == std::string::npos)
        {
            std::string command = "PASS";
            server.replyMissPara(*this, command);
            return(false);
        }
        if (commandReactConnect(server))
            return(true);
        else
            return(false);
    }
    return(true);
   
}
bool    Client::commandReactConnect(Server &server) {

    std::string buffer = this->buffer;
    std::string message;
    std::string prefix;
    std::string command;
    std::string parameter;

    while (!buffer.empty() && buffer.find(DELIMITER) != std::string::npos)
    {
        message = extractMessage(buffer);
        prefix = extractPrefix(message);
        command = extractCommand(message);
        parameter = message;
        if(command == "PASS")
        {
            if (parameter != PASSWORD)
            {
                 server.replyWrongPwd(*this);                 
                 return(false);
            }
        }
        if(command == "NICK") 
        {
            for (size_t i = 0; i < server.getClients().size(); i++)
            {
                if (server.getClients()[i].getNickName() == parameter)
                {
                    server.replyErrNick(*this);
                    return(false);
                }
            }
            this->setNickName(parameter);
            this->setSourceName();
        }
        if(command == "USER")
        {
            commandUser(server, message);
        }  
        buffer = this->buffer;
    }    
    return(true);
}
void    Server::replyWrongPwd(Client &client) {

    std::string const message = ERR_PASSWDMISMATCH(client.getServerName(), client.getNickName());
    sendTemplate(client, message);  

}
void    Server::replyMissPara(Client &client, std::string &command) {

    std::string const message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickName(), command);
    sendTemplate(client, message);
}

void    Server::replyErrRegistered(Client &client) {

    std::string const message = ERR_ALREADYREGISTRED(client.getServerName(), client.getNickName());
    sendTemplate(client, message);
}
void    Server::sendTemplate(Client const &client, std::string const &message)
{
    // std::cout << GREEN << ">> " << message << WHITE << std::flush;
    int sentBytes = send(client.getFd(), message.c_str(), message.length(), 0);
    if (sentBytes == -1)
        throw(std::runtime_error("Failed to send message to client\n")) ;
}
