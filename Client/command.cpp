#include "Client.hpp"

/*   INVITE <nickname> <channel>*/
void	Client::invitation( Server *server, std::string invite ){
	invite = trimSpace(invite);
	size_t index = indexFirstArg( invite );
	
	std::cout << invite << " " << index << " et " << invite.size() << std::endl;

	if ( index == invite.size()){
		send( this->_socket, ERR_NEEDMOREPARAMS(this->_nickname, "INVITE").c_str(), ERR_NEEDMOREPARAMS(this->_nickname, "INVITE").size(), 0);
		return;
	}
	std::string nick = invite.substr(0, index);
	std::string channel = invite.substr( index + 1 );

	// if the client or the channel doesn't exist
	std::vector<Client>::const_iterator it = server->getClients().begin();
	for ( ; it < server->getClients().end(); it++){
		std::cout <<it->getNickname() << " et nick : " << nick << std::endl;
		if ( it->getNickname() == nick )
			break;
	}
	if ( it == server->getClients().end()){
		send(this->_socket, ERR_NOSUCHNICK(this->_nickname, nick).c_str(), ERR_NOSUCHNICK(this->_nickname, nick).size(), 0);
		return ;
	}

	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
		if ( itChan->getName() == channel )
			break;
	if ( itChan == server->getChannels().end() ){
		send(this->_socket, ERR_NOSUCHNICK(this->_nickname, channel).c_str(), ERR_NOSUCHNICK(this->_nickname, channel).size(), 0);
		return ;
	}
	
	// the user is allready on the channel
	std::vector<int>::const_iterator itUser = itChan->getUser().begin();
	for ( ; itUser < itChan->getUser().end(); itUser++ ){
		if ( it->getSocket() == *itUser ){
			send(this->_socket, ERR_USERONCHANNEL(this->_nickname, nick, channel).c_str(), ERR_USERONCHANNEL(this->_nickname, nick, channel).size(), 0);
			return ;
		}
	}

	// int socket  = it->getSocket();
	// itChan->setUserInvite(socket, PUSH);
	send(this->_socket, RPL_INVITING(this->_nickname, nick, channel).c_str(), RPL_INVITING(this->_nickname, nick, channel).size(), 0); 
	send(this->_socket, RPL_INVITED(nick, channel, this->_nickname).c_str(), RPL_INVITED(nick, channel, this->_nickname).size(), 0); 
	/* RPL_INVITING (341) : Indique que l'invitation a été envoyée avec succès à l'utilisateur spécifié.

Exemple :

ruby

:irc.server.com 341 YourNick JohnDoe #example_channel
*/
}


