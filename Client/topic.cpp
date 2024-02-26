#include "Client.hpp"

void	Client::topic( Server *server ){
	if ( this->_splitBuf.size() == 1 ){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}
	std::string tmp = _splitBuf[1];
	size_t delem = tmp.find(" "); 
	if (delem != std::string::npos){
		_splitBuf.pop_back();
		_splitBuf.push_back(tmp.substr(0, delem)); // channel = _splitBuf[1]
		_splitBuf.push_back(tmp.substr(delem)); // new topic = _splitBuf[2]
		_splitBuf[2] = trimSpace(_splitBuf[2]);
	}
	
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
		if ( itChan->getName() == _splitBuf[1] )
			break;
	if ( itChan == server->getChannels().end() ){
		send(this->_socket, ERR_NOSUCHNICK(this->_nickname, _splitBuf[1]).c_str(), ERR_NOSUCHNICK(this->_nickname, _splitBuf[1]).size(), 0);
		return ;
	}

	if ( this->_splitBuf.size() == 2 ){
		if ( itChan->getTopic() == false){
			send(this->_socket, RPL_NOTOPIC(this->_nickname, _splitBuf[1]).c_str(),
					RPL_NOTOPIC(this->_nickname, _splitBuf[1]).size(), 0);
			return;
		}
		std::string topic = itChan->getTopicName();
		send(this->_socket, RPL_TOPIC(this->_nickname, _splitBuf[1], topic).c_str(),
					RPL_TOPIC(this->_nickname, _splitBuf[1], topic).size(), 0);
			return;
	}

	// if user is not in Channel 	
	std::vector<int>::const_iterator it = itChan->getIrcOps().begin();
	for (; it < itChan->getIrcOps().end(); it ++)
		if (*it == this->_socket)
			break;
	if (it == itChan->getIrcOps().end()){
		send(this->_socket, ERR_CHANOPRIVSNEEDED(_nickname, _splitBuf[1]).c_str(),
					ERR_CHANOPRIVSNEEDED(_nickname, _splitBuf[1]).size(), 0);
		return;
	}
	for (size_t i = 0; i < _splitBuf[2].size(); i++){
	       if (!isalnum(_splitBuf[2][i]) && _splitBuf[2][i] != '-' && _splitBuf[2][i] != '_' && _splitBuf[2][i] != '.' && _splitBuf[2][i] != '+'){
			   send (this->_socket, ERR_INVALIDTOPIC(_nickname, _splitBuf[2]).c_str(), ERR_INVALIDTOPIC(_nickname, _splitBuf[2]).size(), 0);
			   return ; 
		   }
	}
	server->changeTopic( _splitBuf[2], _splitBuf[1]);
	send (this->_socket, RPL_TOPIC(_nickname, _splitBuf[1], _splitBuf[2]).c_str(), RPL_TOPIC(_nickname, _splitBuf[1], _splitBuf[2]).size(), 0);
}
