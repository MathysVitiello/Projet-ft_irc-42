#include "../library.hpp"

/* 
MODE #Finnish +im ; Rend le canal #Finnish modéré et 'uniquement sur invitation'.
MODE #Finnish +o Kilroy ; Donne le privilège de 'chanop' à Kilroy sur le canal #Finnish.
MODE #Finnish +v Wiz ; Autorise WiZ à parler sur #Finnish.

ERR_NEEDMOREPARAMS              
ERR_CHANOPRIVSNEEDED //
RPL_CHANNELMODEIS
ERR_NOSUCHNICK
ERR_NOTONCHANNEL                
ERR_KEYSET
RPL_BANLIST                     
RPL_ENDOFBANLIST
ERR_UNKNOWNMODE                 
ERR_NOSUCHCHANNEL //
ERR_USERSDONTMATCH              
RPL_UMODEIS
ERR_UMODEUNKNOWNFLAG

*/
std::string	Client::splitBuf( void )
{
	std::string name;
	size_t		lenName;

	// On stocke le nom du channel:
	this->_splitBuf.erase(this->_splitBuf.begin());
	lenName = this->_splitBuf[0].find(" ");
	name = this->_splitBuf[0];

	std::string tmp = _splitBuf[0];
	this->_splitBuf.pop_back();
	size_t delem = tmp.find(" "); 
	while (delem != std::string::npos){
		_splitBuf.push_back(tmp.substr(0, delem)); // nick = _splitBuf[1]
		tmp.erase( 0, delem + 1);
		delem = tmp.find(" "); 
	}
	_splitBuf.push_back(tmp.substr(0, tmp.size()));
	return (name);
}

// Verifie le nombre de parametre: ERR_NEEDMOREPARAMS              
static bool numberParam( Client *user )
{
	std::cout << "size: " << user->getCmdBuf().size() << std::endl;
	if( user->getCmdBuf().size() > 2 )
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


bool	checkMode( Server *server, Client *user, int i )
{

	// Verification de l operateur:
	// if ( + )
	// 		ftPLUS();
	// else if ( - )
	// 		ft_MOINS();



	if ( user->getCmdBuf()[1] == "+i" || user->getCmdBuf()[1] == "-i" )
		server->channelInvit( user, i);
	// if ( user->getCmdBuf()[2] != "+t" || user->getCmdBuf()[2] != "-t" )
		// modeTopic();
	// if ( user->getCmdBuf()[2] != "+o" || user->getCmdBuf()[2] != "-o" )
		// modePrivilege();
	// if ( user->getCmdBuf()[2] != "+k" || user->getCmdBuf()[2] != "-k" )
		// modePswd();
	else
	{
		std::string nick = user->getNickname();
		std::string server = "irc";
		send(user->getSocket(),ERR_UMODUUNKNOWNFLAG(server, nick).c_str(),
			ERR_UMODUUNKNOWNFLAG(server, nick).size(), 0);
		return( false );
	}

	return( true );
}

void Client::mode( Server *server )
{
	//1- Verifier si le nombre de parametre:
	if ( numberParam( this ) == false )
		return ;

	this->splitBuf();
	std::cout << "chanell name s: " << this->getCmdBuf()[0] << std::endl;
	std::cout << "splitBuf[1]: " << this->getCmdBuf()[1] << std::endl;

	//2- Verifier si le channel existe:
	if( checkChannelExist( server, this->getCmdBuf()[0], this ) == false )
		return;

	//3- Verifier si le client est ircops du channel:
	if( checkIrcOps( server, this->getCmdBuf()[0], this ) == false )
		return;

	// recupere l index du canal dans le tableau:
	int i = 0;
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for (; itChan != server->getChannels().end(); itChan++, i++)
		if( itChan->getName() == this->getCmdBuf()[0] )
			break;

	// Verifie les prefixes:
	if ( checkMode( server, this, i ) == false )
		return;
}



