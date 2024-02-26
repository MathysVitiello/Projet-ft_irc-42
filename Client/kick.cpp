#include "Client.hpp"

void    Client::kick(  Server *server ){


	std::cout << _splitBuf[0] << std::endl;
	std::cout << _splitBuf[1] << std::endl;

	// check si cest un channel
	if (_splitBuf[1][0] == '#' || _splitBuf[1][0] == '&')
	{
		//check channel existe
		std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
		for ( ; itChan < server->getChannels().end(); itChan++)
			if ( itChan->getName() == _splitBuf[1].substr(0 , _splitBuf[1].find(" ")) )
			{
				 //! checker les ops de ce channleel
				std::cout << "le channel existe" << std::endl;

				// send message to the clients of the channel
				//server->sendMessageChanel( nickOrChannel, clientPlace, _splitBuf[1].substr(_splitBuf[1].find(" ")), this->getSocket());
				return;
			}

	} else{

		std::cout << "cest pas un channel" << std::endl;
		return;
	}

	//sasssurer que si le gars est bien Operateur
	// si oui iterer dans le channel jusque trouver le client
	// et le kick (alex function)
}