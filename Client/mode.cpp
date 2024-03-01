#include "../library.hpp"

void	Client::splitBuf( void )
{
	if( this->_splitBuf.size() < 2 )
		return;
	this->_splitBuf.erase(this->_splitBuf.begin());
	std::string tmp = _splitBuf[0];
	this->_splitBuf.pop_back();
	size_t delem = tmp.find(" "); 
	while (delem != std::string::npos)
	{
		_splitBuf.push_back(tmp.substr(0, delem));
		tmp.erase( 0, delem + 1);
		delem = tmp.find(" "); 
	}
	_splitBuf.push_back(tmp.substr(0, tmp.size()));
}

// Verifie le nombre de parametre: ERR_NEEDMOREPARAMS              
static bool numberParam( Client *user )
{
	if( user->getCmdBuf().size() >= 2 )
		return( true );

	std::string mode = "MODE";
	std::string nick = user->getNickname();
	send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, mode).c_str(),
			ERR_NEEDMOREPARAMS(nick, mode).size(), 0);
	return( false );
}

// Verifier si le channel existe: ERR_NOSUCHCHANNEL //
static	bool checkChannelExist( Server *server , std::string nameChan, Client *user )
{
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for (; itChan != server->getChannels().end(); itChan++)
		if( itChan->getName() == nameChan )
			return( true );

	std::string nick = user->getNickname();
	send(user->getSocket(), ERR_NOSUCHCHANNEL(nick, nameChan).c_str(),
			ERR_NOSUCHCHANNEL(nick, nameChan).size(), 0);
	return( false );
}

// Verifier si le user est ircOps: ERR_CHANOPRIVSNEEDED //
static bool checkIrcOps( Server *server, std::string nameChan, Client *user)
{
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();

	for (; itChan != server->getChannels().end(); itChan++)
	{
		if( itChan->getName() == nameChan )
		{
			std::vector<int>::const_iterator itOps = itChan->getIrcOps().begin();
			for (; itOps != itChan->getIrcOps().end(); itOps++)
				if( *itOps == user->getSocket() )
					return( true );
		}
	}
	std::string nick = user->getNickname();
	send(user->getSocket(),ERR_CHANOPRIVSNEEDED(nick, nameChan).c_str(),
			ERR_CHANOPRIVSNEEDED(nick, nameChan).size(), 0);
	return( false );
}

static void	checkMode( Server *server, Client *user, int i )
{
	if ( user->getCmdBuf()[1] == "+i" || user->getCmdBuf()[1] == "-i" )
		server->modeInvit( user, i );
	else if ( user->getCmdBuf()[1] == "+t" || user->getCmdBuf()[1] == "-t" )
		server->modeTopic( user ,i );
	else if ( user->getCmdBuf()[1] == "+o" || user->getCmdBuf()[1] == "-o" )
		server->modePrivilege( user, i );
	else if ( user->getCmdBuf()[1] == "+k" || user->getCmdBuf()[1] == "-k" )
		server->modePswd( user, i );
	else
	{
		std::string nick = user->getNickname();
		std::string server = "irc";
		send(user->getSocket(),ERR_UMODUUNKNOWNFLAG(server, nick).c_str(),
			ERR_UMODUUNKNOWNFLAG(server, nick).size(), 0);
	}
}

void Client::mode( Server *server )
{
	this->splitBuf();
	if ( numberParam( this ) == false )
		return ;

	if( checkChannelExist( server, this->getCmdBuf()[0], this ) == false )
		return;

	if( checkIrcOps( server, this->getCmdBuf()[0], this ) == false )
		return;

	int i = 0;
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for (; itChan != server->getChannels().end(); itChan++, i++)
		if( itChan->getName() == this->getCmdBuf()[0] )
			break;

	checkMode( server, this, i );
}
