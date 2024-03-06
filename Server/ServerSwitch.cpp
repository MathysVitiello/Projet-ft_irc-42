#include "Server.hpp"

void	Server::command(int fdClient){

	std::string	cmd[] = {"CAP", "PASS", "NICK", "USER", "PRIVMSG", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART"};
	int i;

	for (i = 0; i < 11; i++){
		if(this->_clients[fdClient].getCmdBuf().empty()){
			i = -1; 
			break;
		}
		if(this->_clients[fdClient].getCmdBuf()[0] == cmd[i])
			break;
	}

	switch (i) {
	case CAP:
		std::cout << "CAP in switch case " << std::endl;
		this->_clients[fdClient].capForHex( this );
		//this->_clients[fdClient].enterPwd(&this->_clients, this, fdClient);
		break;
	case PASS:
		std::cout << "PASS in switch case " << std::endl;
		this->_clients[fdClient].capForHex( this );
		// ajout ici de remove, jai enlever dans la fiinction pour une raison
		break;
	case NICK:
		std::cout << "NICK in switch case " << this->_clients[fdClient].getCmdBuf()[1] << std::endl;
		this->_clients[fdClient].capForHex( this );
		break;
	case USER:
		std::cout << "USER in switch case " << std::endl;
		this->_clients[fdClient].capForHex( this );
		break;
	case PRIVMSG:
		std::cout << "PRIVMSG in switch case " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].privateMessage(&this->_clients, this, fdClient);
		break;
	case JOIN:
		std::cout << "JOIN in switch case  " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].join(this);
		break;
	case KICK:
		std::cout << "KICK in switch case " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].kick(this);
		break;
	case INVITE:
		std::cout << "INVITE in switch case  " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].invitation( this );
		break;
	case TOPIC:
		std::cout << "TOPIC in switch case " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].topic( this );
		break;
	case MODE:
		std::cout << "MODE in switch case  " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].mode(this);
		break;
	case PART:
		std::cout << " PART in switch case " << std::endl;
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].part(this);
		break;
	default:
		send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		break;
	}
	this->_clients[fdClient].removeCmdBuf();
}