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
		//check channel existe
		std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
		for ( ; itChan < server->getChannels().end(); itChan++)
		{
			if ( itChan->getName() == _splitBuf[1].substr(0 , _splitBuf[1].find(" ")) )
			{
				for (unsigned int i = 0 ; i < itChan->getIrcOps().size(); i++){
					if (itChan->getIrcOps()[i] == this->getSocket())
					{
						if (_splitBuf[2].find(" ") != std::string::npos){
							std::cout << "il y a un message" << std::endl;
							//le nom du boug_splitBuf[1], message dans 2  
							_splitBuf[1] = _splitBuf[2].substr(0, _splitBuf[2].find(" "));
							_splitBuf[2] = _splitBuf[2].substr(_splitBuf[2].find(" "));
							_splitBuf[2] = trimSpace(_splitBuf[2]);
						}
						else {
							_splitBuf[1] = _splitBuf[2];
							_splitBuf[2] = "";
						}
						//trouver son socket a aprtir de son nom
						int socketMan = -1; 
						for (unsigned int i = 0 ; i < server->getClients().size(); i++){
							//found socket of kick man
							if (server->getClients()[i].getName() == _splitBuf[1]){
								socketMan = server->getClients()[i].getSocket();
							}
						}
						if (socketMan == -1){
							//send error();
							std::cout << "le man existe pas" << std::endl;
							return ;
						}

						// find the man in the channel
						for (unsigned int i = 0 ; i < itChan->getUser().size(); i++){
							if (socketMan == itChan->getUser()[i]){

								std::cout << "le man EST VIVANT ET dans le channel" << std::endl;
								//todo donner le message en parametres
								server->kickUser( socketMan, itChan->getName(), _splitBuf[2]);

							}
						}
						// std::cout << "le man nestpas dans le channel" << std::endl;
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

		}
		send(this->_socket, ERR_NOSUCHCHANNEL(this->getNickname(), _splitBuf[1]).c_str(), ERR_NOSUCHCHANNEL(this->getNickname(), _splitBuf[1]).size(), 0);

		//ERR_NOTONCHANNEL you are not in that channel


		std::cout << "cest pas un channel" << std::endl;
		send(this->_socket, ERR_BADCHANMASK(_splitBuf[1]).c_str(), ERR_BADCHANMASK(_splitBuf[1]).size(), 0);

		//ERR_BADCHANMASK
		return;

	//?sasssurer que si le gars est bien Operateur 
	// si oui iterer dans le channel jusque trouver le client
	// et le kick (alex function)	
}
