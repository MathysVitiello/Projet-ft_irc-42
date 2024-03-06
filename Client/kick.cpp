#include "Client.hpp"

void    Client::kick( Server *server ){
	
	if ( this->_splitBuf.size() == 1 ){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}
	std::string tmp = _splitBuf[1];
	size_t delem = tmp.find(" "); 
	if (delem != std::string::npos){
		_splitBuf.pop_back();
		_splitBuf.push_back(tmp.substr(0, delem)); // channel = _splitBuf[1]
		_splitBuf.push_back(tmp.substr(delem)); // user = _splitBuf[2]
		_splitBuf[2] = trimSpace(_splitBuf[2]);
	}
	if ( this->_splitBuf.size() == 2 ){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}
	//check channel exists
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
	{
		if ( itChan->getName() == _splitBuf[1].substr(0 , _splitBuf[1].find(" ")) ||  itChan->getName() == _splitBuf[1])
		{
			for (unsigned int i = 0 ; i < itChan->getIrcOps().size(); i++){

				if (itChan->getIrcOps()[i] == this->getSocket())
				{
					//guy name in _splitBuf[1], message in 2
					if (_splitBuf[2].find(" ") != std::string::npos){
						_splitBuf[1] = _splitBuf[2].substr(0, _splitBuf[2].find(" "));
						_splitBuf[2] = _splitBuf[2].substr(_splitBuf[2].find(" "));
						_splitBuf[2] = trimSpace(_splitBuf[2]);
					}
					else {
						_splitBuf[1] = _splitBuf[2];
						_splitBuf[2] = "";
					}
					int socketMan = -1; 
					//find socket of kick man
					for (unsigned int i = 0 ; i < server->getClients().size(); i++){
						if (server->getClients()[i].getNickname() == _splitBuf[1]){
							socketMan = server->getClients()[i].getSocket();
						}
					}
					if (socketMan == -1){
						send(this->_socket, ERR_NOSUCHNICK(this->getNickname(), _splitBuf[1]).c_str(),
							ERR_NOSUCHNICK(this->getNickname(), _splitBuf[1]).size(), 0);
						return ;
					}
					// find the man in the channel
					for (unsigned int i = 0 ; i < itChan->getUser().size(); i++){
						//once found, kick
						if (socketMan == itChan->getUser()[i]){
							if (this->_socket == socketMan){
								send(this->_socket, "You cannot kick yourself\n", 25, 0);
							} else if (itChan->getOwner() == socketMan){
								send(this->_socket, "You cannot kick the owner\n", 26, 0);
							} else{
								Client client = *this;
								server->kickUser(socketMan, itChan->getName(), _splitBuf[2], client, _splitBuf[1]);
								return;
							}
						}
					}
				}
				else if (i == itChan->getIrcOps().size() - 1) {
					// is not an ops
					send(this->_socket, ERR_CHANOPRIVSNEEDED(this->getNickname(), _splitBuf[1]).c_str(),
						ERR_CHANOPRIVSNEEDED(this->getNickname(), _splitBuf[1]).size(), 0);
				}
			}
			return;
		}
	}
	return;
}