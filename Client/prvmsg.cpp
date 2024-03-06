#include "Client.hpp"

void    Client::privateMessage( std::vector<Client> *clients, Server *server, int clientPlace)
{
	if (_splitBuf[1].find(" ") < _splitBuf[1].find("\n"))
	{
		if (_splitBuf[1].substr(_splitBuf[1].find(" ")).size() == 0){
			send(this->getSocket(), ERR_NOTEXTTOSEND(this->getNickname()).c_str(), ERR_NOTEXTTOSEND(this->getNickname()).size(), 0);
			return ;
		}
		std::string nickOrChannel = _splitBuf[1].substr(0, _splitBuf[1].find(" "));
		//find nickname dans tout les users
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
		{
			if ( nickOrChannel == it->getNickname() ){
				// send message to the client
				std::string toSend = ":" + this->getNickname() + " PRIVMSG "  + it->getNickname() + " " + _splitBuf[1].substr(_splitBuf[1].find(" ")); 
				send(it->getSocket(), toSend.c_str(),toSend.size(), 0); 
				send(it->getSocket(), "\n", 1, 0);
				return ;
			}
		}
		if (nickOrChannel[0] != '#' && nickOrChannel[0] != '&')
			send(this->getSocket(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).c_str(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).size(), 0);

		// check if is channel
		std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
		for ( ; itChan < server->getChannels().end(); itChan++){
			if ( itChan->getName() == nickOrChannel )
			{
				// send message to the clients of the channel
				server->sendMessageChanel( nickOrChannel, clientPlace, _splitBuf[1].substr(_splitBuf[1].find(" ")), this->getSocket());
				return;
			}
		}
		send(server->getClients()[clientPlace].getSocket(), ERR_CANNOTSENDTOCHAN(server->getClients()[clientPlace].getNickname(), nickOrChannel).c_str(),
			ERR_CANNOTSENDTOCHAN(server->getClients()[clientPlace].getNickname(), nickOrChannel).size(), 0);
	} else {
		send(this->getSocket(), ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").c_str(), 
				ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").size(), 0);
		return ;
	}
}

void    Server::sendMessageChanel( std::string nickOrChannel, int clientPlace, std::string cmdSend, int socket)
{
	int nbChannel = -1;
	bool clientInChannel = false;
	// find channel
	for( size_t i = 0; i < this->getChannels().size(); i++ ){

		if (this->getChannels()[i].getName() == nickOrChannel)
			nbChannel = i;
	}

	// find if client is in Channel 
	for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ ){
		if (socket == this->getChannels()[nbChannel].getUser()[i])
			clientInChannel = true;
	}
	if (clientInChannel == false){
		send(this->getClients()[clientPlace].getSocket(), ERR_CANNOTSENDTOCHAN(this->getClients()[clientPlace].getNickname(), nickOrChannel).c_str(),
			ERR_CANNOTSENDTOCHAN(this->getClients()[clientPlace].getNickname(), nickOrChannel).size(), 0);
		return;
	}

	if (this->getChannels()[nbChannel].getUser().size() > 1)
	{
		for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ ){
			if (this->getClients()[i].getNickname() != this->getClients()[clientPlace].getNickname())
			{
				std::string toSend = ":" + this->getClients()[clientPlace].getNickname() + " PRIVMSG " + nickOrChannel + cmdSend + 	"\r\n";
				send(this->getClients()[i].getSocket(), toSend.c_str(),toSend.size(), 0);
			}
		}
	}
	return ;
} 
