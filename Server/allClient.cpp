#include "Server.hpp"

void	Server::allClient(Channel *chan, Client client){
	std::string allUser;
	std::vector<int>::const_iterator itUser;
	
	for ( unsigned int i = 0; i < chan->getUser().size(); i++ ){
		// for ( unsigned int j = 0; j < chan->getIrcOps().size(); j++ ){
		itUser = find( chan->getIrcOps().begin(), chan->getIrcOps().end(), chan->getUser()[i] );	
		if ( itUser != chan->getIrcOps().end() )
			allUser += "@";
		for (unsigned int j = 0; j < this->_clients.size(); j++)
			if (_clients[j].getSocket() == chan->getUser()[i])
				allUser += _clients[j].getNickname();
		allUser += " ";
	}
	send(client.getSocket(), RPL_CHAN(client.getNickname(), "JOIN", chan->getName()).c_str(), RPL_CHAN(client.getNickname(), "JOIN", chan->getName()).size(), 0 );
		send(client.getSocket(), RPL_NAMREPLY( chan->getName(), client.getNickname(), allUser).c_str(), RPL_NAMREPLY( chan->getName(), client.getNickname(), allUser).size(), 0);
	send(client.getSocket(), RPL_ENDOFNAMES(client.getNickname(), chan->getName()).c_str(), RPL_ENDOFNAMES(client.getNickname(), chan->getName()).size(), 0);
}
