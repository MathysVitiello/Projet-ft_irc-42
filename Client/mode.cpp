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


std::string	Client::splitBuf( void )
{
	std::string name;
	size_t		lenName;

	// On stocke le nom du channel:
	lenName = this->getBufTmp().find(" ");
	name = this->_splitBuf[0].substr(5, lenName);

	// on stocke le tout dans splitBuf;
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

void Client::mode( Server *server )
{
	std::string name;

	std::cout << "ligne dans mode: " << this->getBufTmp() << std::endl;
	std::cout << "ligne dans bufTmp: " << this->bufTmp << std::endl;
	std::cout << "ligne dans splitBuff[0]: " << this->_splitBuf[0] << std::endl;
	std::cout << "ligne dans splitBuff[1]: " << this->_splitBuf[1] << std::endl;
	// On stocke le nom du channel et les cmd spliter:
	name = this->splitBuf();

	//1- Verifier si le nombre de parametre:
	if ( numberParam( this ) == false )
		return ;

	//2- Verifier si le channel existe:
	if( checkChannelExist( server, name, this ) == false )
		return;

	//3- Verifier si le client est ircops du channel:
	if( checkIrcOps( server, name, this ) == false )
		return;

	// Verifie les prefixes:
	// definit le canal sur invitation ou non
	int i = 0;
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for (; itChan != server->getChannels().end(); itChan++, i++)
		if( itChan->getName() == this->getCmdBuf()[0] )
			break;
	server->channelInvit( this, i);

}



