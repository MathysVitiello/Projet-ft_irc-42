#include "Client.hpp"

/*   INVITE <nickname> <channel>*/
void	Client::invitation( Server *server ){
	std::string tmp = _splitBuf[1];
	size_t delem = tmp.find(" "); 
	if (delem != std::string::npos){
		_splitBuf[1].erase();
		_splitBuf.push_back(tmp.substr(0, delem)); // nick = _splitBuf[1]
		_splitBuf.push_back(tmp.substr(delem)); // channel = _splitBuf[2]
	}
	if (_splitBuf.size() != 3){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
		return;
	}

	// if the client or the channel doesn't exist
	std::vector<Client>::const_iterator it = server->getClients().begin();
	for ( ; it < server->getClients().end(); it++){
		std::cout <<it->getNickname() << " et nick : " << _splitBuf[1] << std::endl;
		if ( it->getNickname() == _splitBuf[1] )
			break;
	}
	if ( it == server->getClients().end()){
		send(this->_socket, ERR_NOSUCHNICK(this->_nickname, _splitBuf[1]).c_str(), ERR_NOSUCHNICK(this->_nickname, _splitBuf[1]).size(), 0);
		return ;
	}

	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
		if ( itChan->getName() == _splitBuf[2] )
			break;
	if ( itChan == server->getChannels().end() ){
		send(this->_socket, ERR_NOSUCHNICK(this->_nickname, _splitBuf[2]).c_str(), ERR_NOSUCHNICK(this->_nickname, _splitBuf[2]).size(), 0);
		return ;
	}
	
	// the user is allready on the channel
	std::vector<int>::const_iterator itUser = itChan->getUser().begin();
	for ( ; itUser < itChan->getUser().end(); itUser++ ){
		if ( it->getSocket() == *itUser ){
			send(this->_socket, ERR_USERONCHANNEL(this->_nickname, _splitBuf[1], _splitBuf[2]).c_str(), ERR_USERONCHANNEL(this->_nickname, _splitBuf[1], _splitBuf[2]).size(), 0);
			return ;
		}
	}

	// int socket  = it->getSocket();
	// itChan->setUserInvite(socket, PUSH);
	send(this->_socket, RPL_INVITING(this->_nickname, _splitBuf[1], _splitBuf[2]).c_str(), RPL_INVITING(this->_nickname, _splitBuf[1], _splitBuf[2]).size(), 0); 
	send(this->_socket, RPL_INVITED(_splitBuf[1], _splitBuf[2], this->_nickname).c_str(), RPL_INVITED(_splitBuf[1], _splitBuf[2], this->_nickname).size(), 0); 
	/* RPL_INVITING (341) : Indique que l'invitation a été envoyée avec succès à l'utilisateur spécifié.

Exemple :

ruby

:irc.server.com 341 YourNick JohnDoe #example_channel
*/
}


