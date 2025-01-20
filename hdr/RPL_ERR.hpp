#pragma once

#define CRLF "\r\n"


/************************************************                 RESPONSES              **************************************************************/

#define RPL_WELCOME(serverName, nickname) (std::string(":") + serverName + " 001 " + nickname + " :Welcome to the IRC Network " + nickname + CRLF)
#define RPL_YOURHOST(serverName, nickname) (std::string(":") + serverName + " 002 " + nickname + " :Your host is " + serverName + CRLF)

/* AUTOMATIC COMMANDS */
#define RPL_UMODEIS(serverName, nickname, mode) (std::string(":") + nickname + " MODE " + nickname + " :" + mode + CRLF)
#define RPL_ENDOFBANLIST(serverName, nickname, channelName) (std::string(":") + serverName + " 368 " + nickname + " " + channelName + " :End of Channel Ban List" + CRLF)
// #define RPL_WHOREPLY(servername, nickname, username, hostname, realname) (std::string(":") + servername + " 352 " + nickname + " * "+ username + " " + hostname + " " + servername + " " + nickname + " H" + " :0 " + realname+ CRLF)
#define RPL_WHOREPLY(serverName, nickRequest, channelName, nickname, userName, hostName, realName) (std::string(":") + serverName + " 352 " + nickRequest + " " + channelName + " " + userName + " " + hostName + " " + serverName + " " + nickname + " H :0 " + realName + CRLF)
#define RPL_ENDOFWHO(serverName, nickname, channelName) (std::string(":") + serverName + " 315 " + nickname + " " + channelName + " :End of WHO list" + CRLF)

/* COMMAND JOIN */
#define RPL_JOIN(sourceName, channelName) (std::string(":") + sourceName + " JOIN :" + channelName + CRLF)
#define RPL_NAMREPLY(serverName, nickname, channelName) (std::string(":") + serverName + " 353 " + nickname + " = " + channelName + " :")
#define RPL_ENDOFNAMES(serverName, nickname, channelName) (std::string(":") + serverName + " 366 " + nickname + " " + channelName + " :End of /NAMES list" + CRLF)

/* COMMAND KICK */
#define RPL_KICK(sourceName, channelName, recipient, reason) (std::string(":") + sourceName + " KICK " + channelName + " " + recipient + " " + recipient + " :" + reason + CRLF)

/* COMMAND INVITE */
#define RPL_INVITING(serverName, sender, recipient, channelName) (std::string(":") + serverName + " 341 " + sender + " " + recipient + " " + channelName + CRLF)

/* COMMAND TOPIC */
#define RPL_TOPIC(sourceName, channelName, topic) (std::string(":") + sourceName + " TOPIC " + channelName + " :" + topic + CRLF)
// Unnecessary
#define RPL_NOTOPIC(serverName, nickname, channelName) (std::string(":") + serverName + " 331 " + nickname + " " + channelName + " :No topic is set" + CRLF)

/* COMMAND MODE */
// #define RPL_YOUREOPER(servername, nickname,channelname) (std::string(":") + servername + " 381 " + nickname +channelname + " :You are now an IRC operator" + CRLF)
// #define RPL_WHOISOPERATOR(servername, nickname,channelname) (std::string(":") + servername + " 313 " + nickname +" " +nickname+ " :is an IRC operator of #" + channelname+ CRLF)
// #define RPL_ENDOFWHOIS(servername, nickname) (std::string(":") + servername + " 318 " + nickname + " " +nickname+ " :End of WHOIS list" + CRLF)
#define RPL_CHANNELMODEIS(sourceName, channelName, channelMode) (std::string(":") + sourceName + " MODE " + channelName + " " + channelMode + CRLF)
#define RPL_CHANNELMODE(serverName, nickname, channelName, channelmode) (std::string(":") + serverName + " 324 " + nickname + " " + channelName + " " + channelmode + CRLF)

/* COMMAND WHOIS */
// #define RPL_WHOISUSER(servername, nickname, username, hostname, realname) (std::string(":") + servername + " 311 " + nickname + " " + username + " " + hostname + " * :" + realname + CRLF)
// #define RPL_WHOISUSER(servername, nickname, username, hostname) (std::string(":") + servername + " 311 " + nickname + " " + username + " " + hostname + CRLF)


/************************************************                 ERRORS             **************************************************************/

/* COMMAND PRIVMSG / INVITE */
/* ERRORS communes des plusieurs  COMMAND ==> PASS / USER/ OPER /SQUIT /JOIN /PART /MODE /TOPIC /INVITE /KICK /CONNECT et USERMODES  */
#define ERR_NEEDMOREPARAMS(servername, nickname, command) (std::string(":") + servername  + " 461 " + nickname + " " + command + " :Not enough parameters" + CRLF)

/* COMMAND PRIVMSG / KILL / INVITE  ERRORS */
//Used to indicate the nickname parameter supplied to a command is currently unused.
#define ERR_NOSUCHNICK(servername, sender, recipient) (std::string(":") + servername + " 401 " + sender + " " + recipient + " :No such nick/channel" + CRLF )

/* COMMAND PRIVMSG ERRORS */
#define ERR_CANNOTSENDTOCHAN(servername, nickname,channelname) (std::string(":") + servername + " 404 " + nickname + channelname + " :Cannot send to channel" + CRLF) 
 
/* COMMAND PASS / USER ERRORS */
#define ERR_ALREADYREGISTRED(servername, nickname) (std::string(":") + servername  + " 462 " + nickname + " :Unauthorized command (already registered)" + CRLF )

/****** COMMAND MODE -k ERRORS : ********/
/*si oper essaie de modifier la cle sans supprimer l ancienne (commande suppr=>/MODE #channel -k) => operateur fait MODE #channel +k newkey*/
#define ERR_KEYSET(servername,nickname, channelname) (std::string(":") + servername + " 467 " + nickname + " #" + channelname + " :Channel key already set" + CRLF)

/****** COMMAND MODE ERRORS sur le CHANNEL char => mode inconnu par ex j de -j********/
#define ERR_UNKNOWNMODE(servername,nickname,char, channelname) (std::string(":") + servername + " 472 " + nickname + " " + char + " :is unknown mode char to me for " + channelname + CRLF)

/* COMMAND MODE / TOPIC / INVITE ERRORS */
//un utilisateur qui n a pas ls droits. des qu une commande est faite ss les droits oper*/
#define ERR_CHANOPRIVSNEEDED(servername, nickname, channelname) (std::string(":") + servername + " 482 " + nickname + " " + channelname + " :You're not a channel operator" + CRLF)

/* COMMAND OPER ERRORS */
#define ERR_PASSWDMISMATCH(servername,nickname) (std::string(":") + servername + " 464 " + nickname + " :Password incorrect" + CRLF )

/* COMMAND NICK ERRORS */
//Returned when a nickname parameter expected for a command and isn't found.
#define ERR_NONICKNAMEGIVEN(servername, nickname) (std::string(":") + servername  + " 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKNAMEINUSE(servername, nickname) (std::string(":") + servername +" 433 " + nickname + " :Nickname is already in use" + CRLF)
//quand le pseudo est invalid : ne respecte pas les regles 
#define ERR_ERRONEUSNICKNAME(servername,nickname) (std::string(":") + servername  + " 432 " + nickname + " " + nickname +  " :Erroneus nickname" + CRLF)

/**** COMMAND JOIN / PART / KICK / INVITE ERRORS */
// ERR_NOSUCHCHANNEL: nom invalide du channel
#define ERR_NOSUCHCHANNEL(servername, nickname, channelname) (std::string(":") + servername + " 403 " + nickname + " " + channelname + " :No such channel" + CRLF)
#define ERR_TOOMANYCHANNELS(servername,nickname,channelname) (std::string(":") + servername + " 405 " + nickname + " " + channelname + " :You have joined too many channels" + CRLF)
#define ERR_BADCHANNELKEY(servername,nickname,channelname) (std::string(":") + servername + " 475 " + nickname + " " + channelname + " :cannot join channel (+k)" + CRLF)
#define ERR_INVITEONLYCHAN(servername, nickname, channelname) (std::string(":") + servername + " 473 " + nickname + " " + channelname + " :Cannot join channel (+i)" + CRLF)
#define ERR_CHANNELISFULL(servername,nickname,channelname) (std::string(":") + servername + " 471 " + nickname + " " + channelname + " :Cannot join channel (+l)" + CRLF)

/* COMMAND KICK */
#define ERR_USERNOTINCHANNEL(servername, sender, channelname, recipient) (std::string(":") + servername + " 441 " + sender + " " + recipient + " " + channelname + " :They aren't on that channel" + CRLF)

/* COMMAND INVITE ERRORS */
#define ERR_NOTONCHANNEL(servername, nickname, channelname) (std::string(":") + servername + " 442 " + nickname + " " + channelname + " :You're not on that channel" + CRLF)
#define ERR_USERONCHANNEL(servername, nickname, channelname) (std::string(":") + servername + " 443 " + nickname + " " + nickname + " " + channelname + " :is already on channel" + CRLF)

/**** GENERAL ERRORS */
#define ERR_NOTREGISTERED(servername,nickname) (std::string(":") + servername + " 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(servername, nickname, command) (std::string(":") + servername + " 421 " + nickname + " " + command + " :Unknown command" + CRLF)