#include "Server.hpp"

// Rend le channel sur invitation uniquement:
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

// donne/retire les privilèges d'opérateur de canal:
void	Server::modePrivilege( Client *user, int i )
{
	std::string server = "irc";
	std::string nick = user->getNickname();
	std::string channel= this->_channels[i].getName();

	if( user->getCmdBuf().size() != 3 )
	{
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
				ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
		return;
	}

	int flag = 0;
	int index = 0;
	std::vector<Client>::const_iterator it = this->getClients().begin();
	for (; it != this->getClients().end(); it++, index++)
	{
		if( it->getName() == user->getCmdBuf()[2] )
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
	{
		std::cout << "un seul ircops" << std::endl;
		send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
			ERR_MODE(server, channel, nick).size(), 0);
	}
	else if( user->getCmdBuf()[1] == "+o" )	
	{
		std::cout << "mode +o:" << std::endl;
		if( this->_channels[i].addIrcOps(socketClient) == false )
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
				ERR_MODE(server, channel, nick).size(), 0);
	}
	else if( user->getCmdBuf()[1] == "-o" )	
	{
		std::cout << "mode -o:" << std::endl;
		if ( this->_channels[i].removeIrcOps(socketClient) == false )
			send(user->getSocket(), ERR_MODE( server, channel, nick).c_str(), 
				ERR_MODE(server, channel, nick).size(), 0);
	}

}

void	Server::modePswd( Client *user, int i )
{
	(void)user;
	(void)i;
}

