#include "Client.hpp"
#include <iostream>

void Client::setNick(Server *server) {

	if (this->_connected ){
		if (this->_splitBuf.size() == 1 || this->_splitBuf[1].size() == 0){
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, "NICK").c_str(),
					ERR_NEEDMOREPARAMS(this->_nickname, "NICK").size(), 0);
			return;
		}
		if (_splitBuf[1].size() > 9){
			send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
					ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
			return;
		}
		for(size_t i = 0; i <_splitBuf[1].size(); i++){
			if (!isalpha(_splitBuf[1][0])){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
						ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				return;
			}
			if (!isalnum(_splitBuf[1][i]) && 
					_splitBuf[1][i] != '-' && _splitBuf[1][i] != '[' && _splitBuf[1][i] != ']' && _splitBuf[1][i] != '\\' &&  _splitBuf[1][i] != '^' && _splitBuf[1][i] != '_' && _splitBuf[1][i] != '{' && _splitBuf[1][i] != '|' && _splitBuf[1][i] != '}'){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				return;
			}
		}
		std::vector<Client>::const_iterator it = server->getClients().begin(); 
		for ( ;it < server->getClients().end(); it++ )
			if ( _splitBuf[1] == it->getNickname() ){
				send(this->getSocket(),ERR_NICKNAMEINUSE(_splitBuf[1]).c_str(),
						ERR_NICKNAMEINUSE(_splitBuf[1]).size(), 0);
				return;
			}
		this->_nickname = _splitBuf[1];
		if (3 == _splitBuf.size()){
			_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 1);
			server->command(this->_socket);
		}
	}

	if (this->_checkRight == false)
		this->checkRight();
}
