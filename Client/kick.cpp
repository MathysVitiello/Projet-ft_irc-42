#include "Client.hpp"

void    Client::kick(  Server *server ){

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
