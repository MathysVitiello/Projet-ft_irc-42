#include "Client.hpp"

void    Client::privateMessage( std::vector<Client> *clients, Server *server, int clientPlace)
{
	if (_splitBuf[1].find(" ") < _splitBuf[1].find("\n"))
	{
		std::string nickOrChannel = _splitBuf[1].substr(0, _splitBuf[1].find(" "));
		std::cout << "nickOrChannel :" << nickOrChannel << "." << std::endl; 
		//find nickname dans tout les users
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
		{
			if ( nickOrChannel == it->getNickname() ){
				// send message to the client
				if (_splitBuf[1].substr(_splitBuf[1].find(" ")).size() == 0)
					send(this->getSocket(), ERR_NOTEXTTOSEND(this->getNickname()).c_str(), ERR_NOTEXTTOSEND(this->getNickname()).size(), 0);
				else 
					send(it->getSocket(), RPL_AWAY(nickOrChannel , _splitBuf[1].substr(_splitBuf[1].find(" "))).c_str(),
						RPL_AWAY(nickOrChannel, _splitBuf[1].substr(_splitBuf[1].find(" "))).size(), 0);
				return ;
			}
		}
		if (nickOrChannel[0] != '#' && nickOrChannel[0] != '&')
			send(this->getSocket(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).c_str(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).size(), 0);

		// check if is channel
		std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
		for ( ; itChan < server->getChannels().end(); itChan++)
			if ( itChan->getName() == nickOrChannel )
			{
				// send message to the clients of the channel
				server->sendMessageChanel( nickOrChannel, clientPlace, _splitBuf[1].substr(_splitBuf[1].find(" ")), this->getSocket());
				return;
			}
		// send(this->getSocket(), ERR_NOSUCHSERVER(nickOrChannel).c_str(), ERR_NOSUCHSERVER(nickOrChannel).size(), 0);
	} else {
		send(this->getSocket(), ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").c_str(), 
				ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").size(), 0);
		return ;
	}
}
