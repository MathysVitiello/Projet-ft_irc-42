#include "Client.hpp"

void	Client::enterPwd( Server *server ){

    if (this->_connected){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
		return;
	}
	else if (this->_splitBuf.size() != 2){
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}

	std::cout  << "|" << _splitBuf[1] << "| et |" <<  server->getPassword() << "|" << std::endl; //! ICIIII JE SUIS 
	size_t j = _splitBuf[1].find(" ");
	if (j != std::string::npos)
			_splitBuf[1] = trimSpace(_splitBuf[1].substr(j));
	if (_splitBuf[1] == server->getPassword())
		this->_connected = true;
	else if ( server->getPassword() == _splitBuf[1].substr(0, server->getPassword().size()) && _splitBuf[1][server->getPassword().size() + 1] == '\n' ){
		this->_connected = true;
	}
	else{
		send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(),
			ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
	}
}