#include "Client.hpp"

void	Client::join( Server *server )
{
	this->splitBuf();
	if( this->_splitBuf.size() == 1 )
		server->createChannel(*this, _splitBuf[0], "");
	else if( this->_splitBuf.size() == 2 )
		server->createChannel(*this, _splitBuf[0], _splitBuf[1]);
	else
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->getName(), "JOIN").c_str(),
            ERR_NEEDMOREPARAMS(this->getName(), "JOIN").size(), 0);
}
