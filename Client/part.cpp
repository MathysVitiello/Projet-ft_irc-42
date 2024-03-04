#include "Client.hpp"

void	Client::part( Server *server ){
	std::string tmp = _splitBuf[1];
	size_t delem = tmp.find(" "); 
	if (delem != std::string::npos){
		_splitBuf.pop_back();
		_splitBuf.push_back(tmp.substr(0, delem)); // channel = _splitBuf[1]
		_splitBuf.push_back(tmp.substr(delem)); // reason  = _splitBuf[2]
		_splitBuf[2] = trimSpace(_splitBuf[2]);
		std::cout << _splitBuf[2] << std::endl;
	}
	if (_splitBuf.size() == 1){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
		if ( itChan->getName() == _splitBuf[1] )
			break;
	if (itChan == server->getChannels().end()){
		send(this->_socket, ERR_NOSUCHCHANNEL(this->_nickname, _splitBuf[1]).c_str(), ERR_NOSUCHCHANNEL(this->_nickname, _splitBuf[1]).size(), 0);
		return ;	
	}
	for ( unsigned int i = 0; i < itChan->getUser().size(); i++ ){
		if ( itChan->getUser()[i] == this->_socket ){
			if ( _splitBuf.size() == 2 )
				server->part( this->_socket, itChan->getName(), this->_nickname, "" );
			else
				server->part( this->_socket, itChan->getName(), _nickname, _splitBuf[2] );
			return;
		}
	}
	send(this->_socket, ERR_NOTONCHANNEL(this->_nickname, _splitBuf[1]).c_str(), ERR_NOTONCHANNEL(this->_nickname, _splitBuf[1]).size(), 0);
	
}
