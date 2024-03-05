#include "Client.hpp"

void Client::setName( std::vector<Client> *clients, Server *server, int fdClient ) {

	(void)clients;
	(void)server;
	(void)fdClient;

	size_t j = _splitBuf[1].find("USER");
	if (j != std::string::npos)
	{
			_splitBuf[1] = trimSpace(_splitBuf[1].substr(j + 5));
			_splitBuf[1] = _splitBuf[1].substr(0, _splitBuf[1].find(" "));
	}

	if (this->_connected){
		if (this->_splitBuf.size() == 1){
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, "USER").c_str(),
					ERR_NEEDMOREPARAMS(this->_nickname, "USER").size(), 0);
			return;
		}
		else if (this->_name.empty()){
			for(size_t i = 0; i < _splitBuf[1].size(); i++){
				if (!isalnum(_splitBuf[1][i]) && _splitBuf[1][i] != '-' && _splitBuf[1][i] != '[' && _splitBuf[1][i] != ']' 
						&& _splitBuf[1][i] != '\\' &&  _splitBuf[1][i] != '^'
						&& _splitBuf[1][i] != '{' && _splitBuf[1][i] != '|' && _splitBuf[1][i] != '}'){
					send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
							ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
					removeCmdBuf();
					return;
				}
			}
			this->_name = _splitBuf[1];
			if (this->_checkRight == false)
				this->checkRight();
			return;
		}
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
	}
	return ;
}
