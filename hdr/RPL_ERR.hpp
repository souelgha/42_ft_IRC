#pragma once

#define CRLF "\r\n"
/************************************************                 RESPONSES              **************************************************************/
#define RPL_WELCOME(servername, nickname) (std::string(":") + servername + " 001 "  + nickname +" :Welcome to the IRC Network "+ nickname + CRLF)
#define RPL_YOURHOST(servername, nickname) (std::string(":") + servername + " 002 "  + nickname +" :Your host is "+ servername + CRLF)
#define RPL_UMODEIS(servername, nickname, mode) (std::string(":") + servername + " MODE " + nickname + " :" + mode + CRLF)
//NAMEREPLY & ENDOFNAMES vont ensemble
#define RPL_NAMREPLY(servername, nickname, channelname) (std::string(":") + servername + " 353 " + nickname + " @ # *" + channelname + " :" + nickname + CRLF)
#define RPL_ENDOFNAMES(servername, nickname, channelname) (std::string(":") + servername + " 366 " + nickname + " "+channel+ ":End of NAMES list"+ CRLF)
/* COMMAND JOIN TOPIC */
#define RPL_TOPIC(servername, nickname,channelname, topic) (std::string(":") + servername + " 332 " +nickname +" "+channelname + " :" + topic + CRLF)
#define RPL_NOTOPIC(servername, nickname,channelname) (std::string(":") + servername + " 331 " + nickname +" "+channelname + " :No topic is set" + CRLF)
/* COMMAND OPE */
//envoi qd user =>ope dans le channel
#define RPL_YOUREOPER(servername, nickname,channelname) (std::string(":") + servername + " 381 " + nickname +channelname + " :You are now an IRC operator" + CRLF)
#define RPL_WHOISOPERATOR(servername, nickname,channelname) (std::string(":") + servername + " 313 " + nickname +" " +nickname+ " :is an IRC operator of #" + channelname+ CRLF)
#define RPL_ENDOFWHOIS(servername, nickname) (std::string(":") + servername + " 318 " + nickname +" " +nickname+ " :End of WHOIS list" + CRLF)

/* COMMAND INVITE */
#define RPL_INVITING(servername, nickname1, nickname2,channelname) (std::string(":") + servername + " 341 " + nickname2 + " " +nickname1+" INVITE " + nickname2 +" #"+ channelname + CRLF)

/************************************************                 ERRORS             **************************************************************/

/* COMMAND PRIVMSG /INVITE */
/* ERRORS communes des plusieurs  COMMAND ==> PASS / USER/ OPER /SQUIT /JOIN /PART /MODE /TOPIC /INVITE /KICK /CONNECT et USERMODES  */
#define ERR_NEEDMOREPARMs(servername, nickname, command) (std::string(":")+ servername  +" 461 " + nickname + " " + command + " :Not enough parameters" + CRLF)

/* COMMAND PRIVMSG /KILL/INVITE  ERRORS */
//Used to indicate the nickname parameter supplied to a command is currently unused.
#define ERR_NOSUCHNICK(nickname) (std::string(":")+ servername +" 401 " + nickname +" "+ nickname + " :No such nick/channel" + CRLF )
/* COMMAND PRIVMSG  ERRORS */
#define ERR_CANNOTSENDTOCHAN(servername, nickname,channelname) (std::string(":") + servername + " 404 " + nickname + channelname + " :Cannot send to channel" + CRLF) 
 
/* COMMAND PASS /USER ERRORS */
#define ERR_ALREADYREGISTRED(servername, nickname) (std::string(":")+ servername  +" 462 " + nickname + " :Unauthorized command (already registered)" + CRLF )

/****** COMMAND MODE -k ERRORS : ********/
/*si oper essaie de modifier la cle sans supprimer l ancienne (commande suppr=>/MODE #channel -k) => operateur fait MODE #channel +k newkey*/
#define ERR_KEYSET(servername,nickname, channelname) (std::string(":")+ servername +" 467 " + nickname + " #" + channelname + " :Channel key already set" + CRLF)

/****** COMMAND MODE ERRORS sur le CHANNEL char => mode inconnu par ex j de -j********/
#define ERR_UNKNOWNMODE(servername,nickname,char, channelname) (std::string(":")+ servername +" 472"+ nickname + char+ " #" + channelname + " :is unknown mode char to me for " + channelname + CRLF)

// /* COMMAND MODE/TOPIC/INVITE ERRORS */
//un utilisateur qui n a pas ls droits. des qu une commande est faite ss les droits oper*/
#define ERR_CHANOPRIVSNEEDED(servername,nickname,channelname) (std::string(":")+ servername +" 482 "+ nickname +" #" + channelname + " :You're not a channel operator" + CRLF)

/* COMMAND OPER ERRORS */
#define ERR_PASSWDMISMATCH(servername,nickname) (std::string(":")+ servername +" 464 " + nickname + " :Password incorrect" + CRLF )

// /* COMMAND NICK ERRORS */
//Returned when a nickname parameter expected for a command and isn't found.
#define ERR_NONICKNAMEGIVEN(servername, nickname) (std::string(":")+ servername  +" 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKNAMEINUSE(servername, nickname) (std::string(":")+ servername  +" 433 " + nickname +" "+ nickname + " :Nickname is already in use" + CRLF)
//quand le pseudo est invalid : ne respecte pas les regles 
#define ERR_ERRONEUSNICKNAME(servername,nickname) (std::string(":")+ servername  + " 432 " + nickname +" "+ nickname +  " :Erroneus nickname" + CRLF)

// /**** COMMAND JOIN PART KICK ERRORS */
//ERR_NOSUCHCHANNEL nom invalid du channel
#define ERR_NOSUCHCHANNEL(servername,nickname,channelname) (std::string(":")+ servername +" 403 "+ nickname +" " + channelname + " :No such channel" + CRLF)
#define ERR_TOOMANYCHANNELS(servername,nickname,channelname) (std::string(":")+ servername +" 405 "+ nickname +" " + channelname + " :You have joined too many channels" + CRLF)
#define ERR_BADCHANNELKEY(servername,nickname,channelname) (std::string(":")+ servername +" 475 "+ nickname +" " + channelname + " :cannot join channel (+k)" + CRLF)
#define ERR_INVITEONLYCHAN(servername,nickname,channelname) (std::string(":")+ servername +" 473 "+ nickname +" " + channelname + " :Cannot join channel (+l)" + CRLF)
#define ERR_CHANNELISFULL(servername,nickname,channelname) (std::string(":")+ servername +" 471 "+ nickname +" " + channelname + " :Cannot join channel (+i)" + CRLF)


// /* COMMAND INVITE ERRORS */
#define ERR_USERONCHANNEL(servername,nickname,channelname) (std::string(":")+ servername +" 443 "+ nickname +" "+ nickname +" "+ channelname + " :is already on channel" + CRLF)
#define ERR_NOTONCHANNEL(servername,nickname,channelname) (std::string(":")+ servername +" 442 "+ nickname +" "+ channelname + " :You're not on that channel" + CRLF)
#define ERR_USERNOTINCHANNEL(servername,nickname,channelname, nickname2) (std::string(":")+ servername +" 441 "+ nickname +" "+nickname2+" "+ channelname + " :They aren't on that channel" + CRLF)

// /**** GENERAL ERRORS */
#define ERR_NOTREGISTERED(servername,nickname) (std::string(":")+ servername +" 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(servername, nickname, command) (std::string(":")+ servername+" 421 " + nickname + " " + command + " :Unknown command" + CRLF)