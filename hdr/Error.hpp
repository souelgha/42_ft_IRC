#pragma once

#define CRLF "\r\n"

#define RPL_WELCOME(nickname) (": 001 " + ":Welcome to the Internet Relay Network"+ nickname + "!"  + CRLF)
#define RPL_UMODEIS(mode, user) ( ":" + " MODE " + mode + "FOR USER" + user + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
#define RPL_TOPIC(channelname, topic) (": 332 " +channelname + " :" + topic + "\r\n")
#define RPL_YOUREOPER

///////// ERRORS ////////////////
/*/ Where the reply ERR_NOSUCHSERVER is returned, it means that the
   target of the message could not be found.  The server MUST NOT send
   any other replies after this error for that command.*/
#define ERR_NOSUCHSERVER 

/* COMMAND PRIVMSG /KILL ERRORS */
#define ERR_NOSUCHNICK(nickname) (": 401 " + nickname + " :No such nick/channel" + CRLF )

/* ERRORS communes des plusieurs  COMMAND ==> PASS / USER/ OPER /SQUIT /JOIN /PART /MODE /TOPIC /INVITE /KICK /CONNECT et USERMODES  */
#define ERR_NEEDMOREPARMs(command) (": 461 " + nickname + " :Not enough parameters." + CRLF)
/* COMMAND PASS /USER ERRORS */
#define ERR_ALREADYREGISTRED(nickname) (": 462 " + nickname + " :Unauthorized command (already registered)" + CRLF )
/* COMMAND MODE ERRORS */
#define ERR_KEYSET(channelname) (": 467 #" + channelname + " :Channel key already set." + CRLF)
#define ERR_UNKNOWNMODE(channelname) (": 472 #" + channelname + " :is unknown mode char to me for" + channelname + CRLF)
/* COMMAND MODE/TOPIC/INVITE ERRORS */
#define ERR_CHANOPRIVSNEEDED(channelname) (": 482 #" + channelname + " :You're not a channel operator" + CRLF)
/* COMMAND OPER ERRORS */
#define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )

/* NICK ERRORS 1459*/
#define ERR_NONICKNAMEGIVEN(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKNAMEINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICKNAME(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_RESTRICTED
#define ERR_UNAVAILRESOURCE // a voir si necessaire code 437

/**** COMMAND JOIN ERRORS */
#define ERR_NOSUCHCHANNEL 
#define ERR_TOOMANYCHANNELS
#define ERR_BADCHANNELKEY
#define ERR_INVITEONLYCHAN 
#define ERR_CHANNELISFULL 

/**** GENERAL ERRORS */
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)