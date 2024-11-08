#include "Server.hpp"

// Makes the channel by invitation only (i): RPL_CHANNELMODEIS //
void	Server::modeInvit( Client *user, int i )
{
	std::string nick = user->getNickname();
	std::string server = "irc";
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf().size() > 2 )
	{
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
				ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
		return;
	}

	if( user->getCmdBuf()[1] == "+i" )	
	{
		if( this->_channels[i].getInvitation() == false )
		{
			this->_channels[i].setInvitation( true );
			send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "+i").c_str(), 
				RPL_CHANNELMODEIS(nick, channel, "+i").size(), 0);
		}
		else
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
	}
	else if( user->getCmdBuf()[1] == "-i" )	
	{
		if( this->_channels[i].getInvitation() == true )
		{
			send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "-i").c_str(), 
				RPL_CHANNELMODEIS(nick, channel, "-i").size(), 0);
			this->_channels[i].setInvitation( false );
		}
		else
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
	}
}

// change topic privilege (t) //
void	Server::modeTopic( Client *user, int nChannel )
{
	std::string server 	= "irc";
	std::string nick 	= user->getNickname();
	std::string channel	= this->_channels[nChannel].getName();

	if( user->getCmdBuf().size() > 2 )
	{
		std::string mode = "MODE";
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, mode).c_str(),
				ERR_NEEDMOREPARAMS(nick, mode).size(), 0);
		return;
	}
	
	if( user->getCmdBuf()[1] == "+t" )	
	{
		if(this->_channels[nChannel].getTopicPrivilege() == true)
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
		else
		{
			this->_channels[nChannel].setTopicPrivilege( true );
			send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "+t").c_str(), 
					RPL_CHANNELMODEIS(nick, channel, "+t").size(), 0);
		}
	}
	else if( user->getCmdBuf()[1] == "-t" )	
	{
		if(this->_channels[nChannel].getTopicPrivilege() == false)
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
					ERR_MODE(server, channel, nick).size(), 0);
		else
		{
			this->_channels[nChannel].setTopicPrivilege( false );
			send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "-t").c_str(), 
					RPL_CHANNELMODEIS(nick, channel, "-t").size(), 0);
		}
	}
}

// Gives/removes channel operator privileges (o): RPL_CHANNELMODEIS2 //
void	Server::modePrivilege( Client *user, int i )
{
	std::string server = "irc";
	std::string nick 	= user->getNickname();
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf().size() != 3 )
	{
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
				ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
		return;
	}

	int index = 0;
	int flag = 0;
	std::vector<Client>::const_iterator it = this->getClients().begin();
	for (; it != this->getClients().end(); it++, index++)
	{
		if( it->getNickname() == user->getCmdBuf()[2] )
		{
			flag = 1;
			break;
		}
	}
	if( flag == 0 )
	{
		send(user->getSocket(), ERR_NOSUCHNICK(user->getCmdBuf()[2], 
			channel).c_str(), ERR_NOSUCHNICK(user->getCmdBuf()[2], channel).size(), 0);
		return;
	}

	int socketClient = this->getClients()[index].getSocket();
	if( socketClient == user->getSocket() && this->_channels[i].getIrcOps().size() == 1 )
		send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
			ERR_MODE(server, channel, nick).size(), 0);
	else if( user->getCmdBuf()[1] == "+o" )	
	{
		if( this->_channels[i].addIrcOps(socketClient) == false )
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
				ERR_MODE(server, channel, nick).size(), 0);
		else
		{
			for(size_t j = 0; j < this->_channels[i].getUser().size(); j++ )
				send(this->_channels[i].getUser()[j], RPL_CHANNELMODEIS2(nick, channel, "+o", user->getCmdBuf()[2]).c_str(), 
					RPL_CHANNELMODEIS2(nick, channel, "+o", user->getCmdBuf()[2]).size(), 0);

		}
	}
	else if( user->getCmdBuf()[1] == "-o" )	
	{
		if ( this->_channels[i].removeIrcOps(socketClient) == false )
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
				ERR_MODE(server, channel, nick).size(), 0);
		else
		{
			for(size_t j = 0; j < this->_channels[i].getUser().size(); j++ )
				send(this->_channels[i].getUser()[j], RPL_CHANNELMODEIS2(nick, channel, "-o", user->getCmdBuf()[2]).c_str(), 
					RPL_CHANNELMODEIS2(nick, channel, "-o", user->getCmdBuf()[2]).size(), 0);
		}
	}
}

// Add or remove channel password (k): RPL_CHANNELMODEIS2 //
void	Server::modePwd( Client *user, int i )
{
	std::string server = "irc";
	std::string nick = user->getNickname();
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf()[1] == "+k" && user->getCmdBuf().size() == 3 )	
	{
		this->_channels[i].setPassword( user->getCmdBuf()[2] );
		this->_channels[i].setPwd( true );		
		send(user->getSocket(), RPL_CHANNELMODEIS2(nick, channel, "+k", user->getCmdBuf()[2]).c_str(), 
				RPL_CHANNELMODEIS2(nick, channel, "+k", user->getCmdBuf()[2]).size(), 0);
	}
	else if( user->getCmdBuf()[1] == "-k" && user->getCmdBuf().size() == 2 )	
	{
		this->_channels[i].setPassword( "" );
		this->_channels[i].setPwd( false );		
		send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "-k").c_str(), 
				RPL_CHANNELMODEIS(nick, channel, "-k").size(), 0);
	}
	else
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
			ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
}

// Define the user's limit in the channel (l):
void Server::modeLimitUser( Client *user, int i )
{
	std::string server = "irc";
	std::string nick = user->getNickname();
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf()[1] == "+l" && user->getCmdBuf().size() == 3 )	
	{
		char	*pEnd;
		long int limit = strtol( user->getCmdBuf()[2].c_str(), &pEnd, 10 );
		if ( limit > 1024 || limit < 1 )
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
				ERR_MODE(server, channel, nick).size(), 0);
		else
		{
			this->_channels[i].setMaxUser( limit );		
			send(user->getSocket(), RPL_CHANNELMODEIS(nick, channel, "+l").c_str(), 
				RPL_CHANNELMODEIS(nick, channel, "+l").size(), 0);
		}
	}
	else if( user->getCmdBuf()[1] == "-l" && user->getCmdBuf().size() == 2 )	
		this->_channels[i].setMaxUser( 1024 );
	else
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
			ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
}
