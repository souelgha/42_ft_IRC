#pragma once

#define CRLF "\r\n"

#define RPL_WELCOME(servername, nickname) (std::string(":") + servername + " 001 "  + nickname +" :Welcome to the IRC Network "+ nickname + CRLF)
#define RPL_UMODEIS(servername, nickname, mode) (std::string(":") + servername + " MODE " + nickname + " :" + mode + CRLF)
// #define RPL_NAMREPLY(servername, nickname, channelname) (std::string(":") + servername + " 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
// #define RPL_TOPIC(channelname, topic) (": 332 " +channelname + " :" + topic + "\r\n")
// #define RPL_YOUREOPER

///////// ERRORS ////////////////

/* ERRORS communes des plusieurs  COMMAND ==> PASS / USER/ OPER /SQUIT /JOIN /PART /MODE /TOPIC /INVITE /KICK /CONNECT et USERMODES  */
#define ERR_NEEDMOREPARMs(servername, nickname, command) (std::string(":")+ servername  +" 461 " + nickname + " " + command + " :Not enough parameters" + CRLF)


/* COMMAND PRIVMSG /KILL ERRORS */
// #define ERR_NOSUCHNICK(nickname) (std::string(":")+" 401 " + nickname + " :No such nick/channel" + CRLF )


/* COMMAND PASS /USER ERRORS */
#define ERR_ALREADYREGISTRED(servername, nickname) (std::string(":")+ servername  +" 462 " + nickname + " :Unauthorized command (already registered)" + CRLF )
/* COMMAND MODE ERRORS */
// #define ERR_KEYSET(channelname) (": 467 #" + channelname + " :Channel key already set." + CRLF)
// #define ERR_UNKNOWNMODE(channelname) (": 472 #" + channelname + " :is unknown mode char to me for" + channelname + CRLF)
// /* COMMAND MODE/TOPIC/INVITE ERRORS */
// #define ERR_CHANOPRIVSNEEDED(channelname) (": 482 #" + channelname + " :You're not a channel operator" + CRLF)
// /* COMMAND OPER ERRORS */
// #define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )

// /* NICK ERRORS 1459*/
//Returned when a nickname parameter expected for a command and isn't found.
#define ERR_NONICKNAMEGIVEN(servername, nickname) (std::string(":")+ servername  +" 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKNAMEINUSE(servername, nickname) (std::string(":")+ servername  +" 433 " + nickname +" "+ nickname + " :Nickname is already in use" + CRLF)
//quand le pseudo est invalid : ne respecte pas les regles 
#define ERR_ERRONEUSNICKNAME(servername,nickname) (std::string(":")+ servername  + " 432 " + nickname +" "+ nickname +  " :Erroneus nickname" + CRLF)

// /**** COMMAND JOIN ERRORS */
#define ERR_NOSUCHCHANNEL 
#define ERR_TOOMANYCHANNELS
#define ERR_BADCHANNELKEY
#define ERR_INVITEONLYCHAN 
#define ERR_CHANNELISFULL 

// /**** GENERAL ERRORS */
// #define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
// #define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)