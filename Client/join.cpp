#include "Client.hpp"

// creating or joining a channel //
void	Client::join( Server *server )
{
	this->splitBuf();
	if (this->_splitBuf[0][0] != '#' && this-> _splitBuf[0][0] != '&')
	{
		send(this->getSocket(), ERR_NOSUCHCHANNEL(this->getNickname(), this->_splitBuf[1]).c_str(),
            ERR_NOSUCHCHANNEL(this->getNickname(), this->_splitBuf[1]).size(), 0);
	}
	else if( this->_splitBuf.size() == 1 )
		server->createChannel(*this, _splitBuf[0], "");
	else if( this->_splitBuf.size() == 2 )
		server->createChannel(*this, _splitBuf[0], _splitBuf[1]);
	else
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->getNickname(), "JOIN").c_str(),
            ERR_NEEDMOREPARAMS(this->getNickname(), "JOIN").size(), 0);
}
