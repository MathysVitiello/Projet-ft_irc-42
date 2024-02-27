#include "Client.hpp"

void Client::setNick( std::vector<Client> *clients, Server *server, int fdClient ) {

	size_t j = _splitBuf[1].find("NICK");
	if (j != std::string::npos)
	{
			_splitBuf[0] = _splitBuf[1];
			_splitBuf[1] = trimSpace(_splitBuf[1].substr(j + 5));
			_splitBuf[1] = _splitBuf[1].substr(0, _splitBuf[1].find("\r"));
	}

	if (this->_connected){
		if (_splitBuf[1].empty()){
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, "NICK").c_str(),
					ERR_NEEDMOREPARAMS(this->_nickname, "NICK").size(), 0);
			removeCmdBuf();
			return;
		}
		if (_splitBuf[1].size() > 9){
			send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
					ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
			removeCmdBuf();
			return;
		}
		for(size_t i = 0; i <_splitBuf[1].size(); i++){
			if (!isalpha(_splitBuf[1][0])){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
						ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
			if (!isalnum(_splitBuf[1][i]) && 
					_splitBuf[1][i] != '-' && _splitBuf[1][i] != '[' && _splitBuf[1][i] != ']' && _splitBuf[1][i] != '\\' &&  _splitBuf[1][i] != '^' && _splitBuf[1][i] != '_' && _splitBuf[1][i] != '{' && _splitBuf[1][i] != '|' && _splitBuf[1][i] != '}'){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
		}
		std::vector<Client>::const_iterator it = server->getClients().begin(); 
		for ( ;it < server->getClients().end(); it++ )
			if ( _splitBuf[1] == it->getNickname() ){
				send(this->getSocket(),ERR_NICKNAMEINUSE(_splitBuf[1]).c_str(),
						ERR_NICKNAMEINUSE(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
		this->_nickname = _splitBuf[1];
		if (3 == _splitBuf.size()){
			_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 1);
			server->command(this->_socket);
		}
	}

	size_t k = _splitBuf[0].find("\r"); // condition hexchat
	if (k != std::string::npos)
	{
		_splitBuf[1] = _splitBuf[0].substr(_splitBuf[0].find("\r"));
		_splitBuf[1] = _splitBuf[1].substr(_splitBuf[1].find("\r"));
		_splitBuf[0] = "USER";
		this->setName(clients, server, fdClient); 
	}
	removeCmdBuf();
}