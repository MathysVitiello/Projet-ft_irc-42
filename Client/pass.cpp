#include "Client.hpp"

void	Client::enterPwd(std::vector<Client> *clients, Server *server, int fdClient ){
		
    if (this->_connected){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
        removeCmdBuf();
		return;
	}
	else if (this->_splitBuf.size() != 2){
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
    	removeCmdBuf();
		return;
	}
	size_t j = _splitBuf[1].find(" ");
	if (j != std::string::npos)
			_splitBuf[1] = trimSpace(_splitBuf[1].substr(j));

	if (_splitBuf[1] == server->getPassword()){
		this->_connected = true;
	}
	else if ( server->getPassword() == _splitBuf[1].substr(0, server->getPassword().size()) && _splitBuf[1][server->getPassword().size() + 1] == '\n' ){

		this->_connected = true;
		//if there is NICK after, for hexchat
		if (_splitBuf[1].size() != server->getPassword().size() + 1)
			this->setNick(clients, server, fdClient);
	}
	else{
		send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(),
				ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
	}
	removeCmdBuf();
}