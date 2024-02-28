#include "Server.hpp"

void	Server::modeInvit( Client *user, int i )
{
	std::string nick = user->getNickname();
	std::string server = "irc";
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf().size() > 2 )
	{
		std::string mode = "MODE";
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, mode).c_str(),
				ERR_NEEDMOREPARAMS(nick, mode).size(), 0);
		return;
	}

	if( user->getCmdBuf()[1] == "+i" )	
	{
		if( this->_channels[i].getInvitation() == false )
			this->_channels[i].setInvitation( true );
		else
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
	}
	else if( user->getCmdBuf()[1] == "-i" )	
	{
		if( this->_channels[i].getInvitation() == true )
			this->_channels[i].setInvitation( false );
		else
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
	}
}

void	Server::modeTopic( Client *user, int nChannel )
{
	(void)user;
	(void)nChannel;
}
