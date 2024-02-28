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
				for (unsigned int i = 0 ; i < itChan->getIrcOps().size(); i++){

					if (itChan->getIrcOps()[i] == this->getSocket())
					{
						//todo
						if (_splitBuf[2].find(" ") != std::string::npos){

							std::cout << "il y a un message" << std::endl;
							//le nom du boug_splitBuf[1], message dans 2  
							_splitBuf[1] = _splitBuf[2].substr(0, _splitBuf[2].find(" "));
							_splitBuf[2] = _splitBuf[2].substr(_splitBuf[2].find(" "));
							_splitBuf[2] = trimSpace(_splitBuf[2]);
							
							
							std::cout << "nom ="<< _splitBuf[1] << std::endl;

						}
						else {

							_splitBuf[1] = _splitBuf[2];
							_splitBuf[2] = "";

							//le nom du boug_splitBuf[2] =  
							std::cout << "no message" << std::endl;
							std::cout << "nom ="<< _splitBuf[1] << std::endl;

						}

						//! chercher le boug de pslit[1]

						for (unsigned int i = 0 ; i < itChan->getUser().size(); i++){
//todo jai tout les socket JE SUIS ICI JE CHERCHE A TROUVER LE NOM DU GARS MAIS JAI QUE SON SOCKET
							std::cout << "nom1 ="<< itChan->getUser()[i] << std::endl;

							// if ( _splitBuf[1] == server->getChannels[]      itChan->getUser()[i] ) //todo
							// {
							// 	std::cout << "trouve le bOUG cest : " << _splitBuf[1] << std::endl;

							// }
						}
						std::cout << "pas trouve le boug" << std::endl;

						
						


						// checker si il y a un message ou non et kick et lui envoye le message
						// ajout message que le kick a ete fait avec succes
					}
					else {
						// is not an ops
						send(this->_socket, ERR_CHANOPRIVSNEEDED(this->getNickname(), _splitBuf[1]).c_str(),
							ERR_CHANOPRIVSNEEDED(this->getNickname(), _splitBuf[1]).size(), 0);
					}
				}
				return;
			}
		send(this->_socket, ERR_NOSUCHCHANNEL(this->getNickname(), _splitBuf[1]).c_str(), ERR_NOSUCHCHANNEL(this->getNickname(), _splitBuf[1]).size(), 0);

		//ERR_NOTONCHANNEL you are not in that channel

	} else{

		std::cout << "cest pas un channel" << std::endl;
		send(this->_socket, ERR_BADCHANMASK(_splitBuf[1]).c_str(), ERR_BADCHANMASK(_splitBuf[1]).size(), 0);

		//ERR_BADCHANMASK
		return;
	}

	//?sasssurer que si le gars est bien Operateur 
	// si oui iterer dans le channel jusque trouver le client
	// et le kick (alex function)	
}
