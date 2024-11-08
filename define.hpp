#ifndef DEFINE_HPP
#define DEFINE_HPP

/* PASS message  */
# define ERR_NEEDMOREPARAMS(nick, command)	    (": 461 "+nick+" :"+command+" :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(nick)			(": 462 "+nick+" :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH(nick)				(": 464 "+nick+" :Password incorrect\r\n")

/*  NICK message */
# define ERR_NONICKNAMEGIVEN (nick)				(": 431 "+nick+" :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick)				(": 432 "+nick+" :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick)				(": 433 "+nick+" :Nickname is already in use\r\n")
# define ERR_NICKCOLLISION(nick)				(": 436 "+nick+" :Nickname collision KILL from <user>@<host>\r\n")
# define RPL_NICKCHANGE(oldnick, newnick)		(":"+ oldnick+" NICK :"+newnick+"\r\n")
                                      
/*	CHANNEL message */                                           
# define CHANNELMADE(channel)			            ("001 "+channel+", Channel created\r\n") //homemade
# define ERR_NOSUCHCHANNEL(nick, channel)			(": 403 "+nick+" "+channel+" :No such channel\r\n")
# define ERR_TOOMANYCHANNELS(nick, channel)			(": 405 "+nick+" "+channel+" :You have joined too many channels\r\n")
# define ERR_BADCHANNELKEY(nick, channel)			(": 475 "+nick+" "+channel+" :Cannot join channel (+k)\r\n")
# define ERR_BANNEDFROMCHAN(nick, channel)			(": 474 "+nick+" "+channel+" :Cannot join channel (+b)\r\n")
# define ERR_CHANNELISFULL(nick, channel)			(": 471 "+nick+" "+channel+" :Cannot join channel (+l)\r\n")
# define ERR_INVITEONLYCHAN(nick, channel)			(": 473 "+nick+" "+channel+" :Cannot join channel (+i)\r\n")
# define ERR_BADCHANMASK(channel)				    (": 476 "+channel+" :Bad Channel Mask\r\n" )

# define RPL_TOPIC(nick, channel, topic)		    (": 332 "+nick+" "+channel+" "+topic+"\r\n")
# define RPL_CHAN(client, cmd, channel) 		    (":"+client+" "+cmd+" "+channel +"\r\n")
# define RPL_NAMREPLY(channel, client, allClient) 	(": 353 " +client+" = "+channel+" :"+allClient +"\r\n")
# define RPL_ENDOFNAMES(nick, channel)	        	(": 366 "+nick+" "+channel+" :End of NAMES list\r\n")
# define RPL_INVITATION(nick , channel, cmd)		(":"+nick+" MODE "+channel+" "+cmd+"\r\n" )     
                                      
/*	TOPIC message	*/                
# define RPL_NOTOPIC(nick, channel)				(": 331 "+nick+" "+channel+" There is no topic yet\r\n")                                     
# define ERR_NOTONCHANNEL(nick, channel)	    (": 442 "+nick+" "+channel+" :You're not on that channel\r\n")
# define ERR_CHANOPRIVSNEEDED(nick, channel)	(": 482 "+nick+" "+channel+" :You're not channel operator\r\n")
                                     
/* INVITE message	*/               
# define RPL_INVITING(nick, guest, channel)			(": 341 "+nick+" invited "+guest+" to the channel "+channel+"\r\n")
# define RPL_INVITED(guest, channel, nick)			(": 345 "+guest+" is invited on "+channel+" by "+nick+"\r\n")
# define ERR_CHANOPRIVSNEEDED(nick, channel)		(": 482 "+nick+" "+channel+" :You're not channel operator\r\n")
# define ERR_USERONCHANNEL(server, nick, channel)	(": 443 "+server+" "+nick+" is already on channel :"+channel+"\r\n")
# define ERR_INVALIDTOPIC(nick, channel)			(": 442 "+nick+" "+channel+" :Invalid characters in topic\r\n")
                                      
/*	PRIVMSG message	*/                
# define ERR_NOSUCHNICK(nick, guest)		    (": 401 "+nick+" "+guest+" :No such nick\r\n")
# define ERR_CANNOTSENDTOCHAN(nick, channel)	(": 404 "+nick+" "+channel+" :Cannot send to channel\r\n")
# define ERR_NORECIPIENT(nick, command)			(": 411 "+nick+" :No recipient given (" +command+")\r\n")//? DONE
# define ERR_NOTEXTTOSEND(nick)					(": 412 "+nick+" :No text to send\r\n")//? DONE
# define RPL_AWAY(nick, message)				(": 301 "+nick+" :"+message+"\r\n")//? DONE
                                      
/* PRIVILEGE message	*/                                  
# define ERR_NOPRIVILEGES(nick)				            (": 481 "+nick+" :Permission Denied- You're not an IRC operator\r\n")
# define RPL_CHANNELMODEIS(nick, channel, mode)		    (": 324 "+nick+" "+channel+" "+mode+"\r\n")
# define RPL_CHANNELMODEIS2(nick, channel, cmd1, cmd2)	(": 324 "+nick+" "+channel+" "+cmd1+" "+cmd2+"\r\n")
# define ERR_NOPRIVS(nick)					            (": 723 "+nick+" <priv> :Insufficient oper privileges.\r\n")

/*	MODES		*/  
# define ERR_UMODUUNKNOWNFLAG(server, nick)			(": 501 "+server+" "+nick+" :Unknown MODE flag\r\n" )
# define ERR_MODE(server, channel, nick)			(": 502 "+server+" "+nick+": "+channel+" :Cannot change mode \r\n" )                    
                                      
/*	NUMERICS	*/                                     
# define RPL_WELCOME(nick, user)				(": 001 "+nick+" :Welcome to the IRC Network, "+nick+" ! "+user+"\r\n")
# define ERR_UNKNOWNCOMMAND(nick)			 	(": 421 "+nick+" :Unknown command\r\n")
# define RPL_PART( nick, channel, reason )		(":"+nick+" PART "+channel+" the channel "+reason+"\n\r")
# define NEWTOCHANNEL(nick, channel)			(": "+nick+" has joined channel : "+channel+"\r\n")
#endif
