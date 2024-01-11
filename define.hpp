#ifndef DEFINE_HPP
#define DEFINE_HPP

# define ERR_NOMOREFDFREE						("421 :Too many connections, try again later\r\n")
/* PASS message  */

# define ERR_NEEDMOREPARAMS(nick, command)			("461 : "+nick+" :"+command+" :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(nick)					("462 : "+nick+" :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH(nick)					("464 : "+nick+" :Password incorrect\r\n")

/*  NICK message */

# define ERR_NONICKNAMEGIVEN (nick)					("431 : "+nick+" :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick)					("432 : "+nick+" :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick)					("433 : "+nick+" :Nickname is already in use\r\n")
# define ERR_NICKCOLLISION(nick)					("436 : "+nick+" :Nickname collision KILL from <user>@<host>\r\n")
                                      
/*	JOIN message */                   
                                      

# define ERR_NOSUCHCHANNEL(nick, channel)			("403 : "+nick+" "+channel+" :No such channel\r\n")
# define ERR_TOOMANYCHANNELS(nick, channel)			("405 : "+nick+" "+channel+" :You have joined too many channels\r\n")
# define ERR_BADCHANNELKEY(nick, channel)			("475 : "+nick+" "+channel+" :Cannot join channel (+k)\r\n")
# define ERR_BANNEDFROMCHAN(nick, channel)			("474 : "+nick+" "+channel+" :Cannot join channel (+b)\r\n")
# define ERR_CHANNELISFULL(nick, channel)			("471 : "+nick+" "+channel+" :Cannot join channel (+l)\r\n")
# define ERR_INVITEONLYCHAN(nick, channel)			("473 : "+nick+" "+channel+" :Cannot join channel (+i)\r\n")
# define ERR_BADCHANMASK(channel)				("476 : "+channel+" :Bad Channel Mask" )


# define RPL_TOPIC(nick, channel, topic)	("332 : "+nick+" "+channel+" :"+topic+"\r\n")
# define RPL_NAMREPLY(nick, channel)			("353 : "+nick+" <symbol> "+channel+" :[prefix]<nick>{ [prefix]<nick>}\r\n")
# define RPL_ENDOFNAMES(nick, channel)			("366 : "+nick+" "+channel+" :End of /NAMES list\r\n")
                                      
/*	TOPIC message	*/                
                                      
# define ERR_NOTONCHANNEL(nick, channel)			("442 : "+nick+" "+channel+" :You're not on that channel\r\n")
# define ERR_CHANOPRIVSNEEDED(nick, channel)			("482 : "+nick+" "+channel+" :You're not channel operator\r\n")
                                     
/* INVITE message	*/               

# define RPL_INVITING(nick, channel)			("341 : "+nick+" invited "+guest+" to the channel "+channel+"\r\n")

# define ERR_CHANOPRIVSNEEDED(nick, channel)			("482 : "+nick+" "+channel+" :You're not channel operator\r\n")
# define ERR_USERONCHANNEL(nick, channel)			("443 : "+nick+" :is already on channel :"+channel+"\r\n")
                                      
/*	PRIVMSG message	*/                
# define ERR_NOSUCHNICK(nick)					("401 : "+nick+" <nickname> :No such nick/channel\r\n")
# define ERR_NOSUCHSERVER(nick)					("402 : "+nick+" <server name> :No such server\r\n")
# define ERR_CANNOTSENDTOCHAN(nick)					("404 : "+nick+" "+channel+" :Cannot send to channel\r\n")
# define ERR_NORECIPIENT(nick)					("411 : "+nick+" :No recipient given (<command>)\r\n")
# define ERR_NOTEXTTOSEND(nick)					("412 : "+nick+" :No text to send\r\n")

# define RPL_AWAY(nick)					("301 : "+nick+" <nick> :<message>\r\n")
                                      
/*	KILL message	*/                
                                     
# define ERR_NOPRIVILEGES(nick)					("481 : "+nick+" :Permission Denied- You're not an IRC operator\r\n")
# define ERR_NOPRIVS(nick)					("723 : "+nick+" <priv> :Insufficient oper privileges.\r\n")
                                      
/*	NUMERICS	*/                    
                                      
# define RPL_WELCOME(nick)					("001 : "+nick+" :Welcome to the IRC Network, "+nick+" !<user>@<host>]\r\n")


#endif
