#include "Client.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Client::Client( int const & id, sockaddr_in from ) :	_socket( id ),
														_address( from ),
														_connected(false),
														_checkRight(false){
	std::cout << "Client constructor called" << std::endl;
	std::cout << " id:  " << this->_socket << std::endl;
	std::cout << " reseau :  " << this->_address.sin_port << std::endl;
	this->_name = "";
	this->_nickname = "*";
	return ;
}

Client::~Client( void ) {
	std::cout << "Client Destructor called" << std::endl;
	return ;
}
/* ************************************************************************** */
// OPERATOR OVERLOAD:


/* ************************************************************************** */
// ACCESSORS:
int	const & Client::getSocket( void ) const{
	return( this->_socket );
}

sockaddr_in const & Client::getAddr( void ) const{
	return( this->_address );
}

std::string const & Client::getName( void ) const{
	return( this->_name );
}

std::string const & Client::getNickname( void ) const{
	return( this->_nickname );
}

bool	const & Client::getConnect( void ) const{
	return( this->_connected );
}

bool	const & Client::getConnectServer( void ) const{
	return( this->_checkRight );
}

std::vector<std::string>	const & Client::getCmdBuf(void) const{
	return this->_splitBuf;
}

std::string		Client::getBufTmp( void ){
	return this->bufTmp;
}

void	Client::setSocket( int socket ){
	this->_socket = socket;
}

void	Client::checkRight( void ) {
	if (this->_connected == true && this->getName() != "" && this->getNickname() != "*" && this->_checkRight == false){
		send(this->getSocket(),	RPL_WELCOME(this->_nickname, this->_name).c_str(),
				RPL_WELCOME(this->_nickname, this->_name).size(), 0);
		this->_checkRight = true;
	}
}

void Client::setAddr( sockaddr_in addr ) {
	this->_address = addr;
	return ;
}

void	Client::setBufTmp( std::string buf, int flag ){
	if ( flag == PUSH )
		this->bufTmp += buf;
	else
		this->bufTmp.erase(bufTmp.begin(), bufTmp.end());
}

/* ************************************************************************** */
// FONCTIONS:

//parsing depending on hexchat
void	Client::capForHex( Server *server, int fdClient, std::vector<Client> *clients){

		//? FONCTIONNE
		if (bufTmp.find("CAP ") == 0)
		{

			std::cout << "ici1" << std::endl;

			size_t j = bufTmp.find("PASS");
			if (j == std::string::npos)
				return;
			bufTmp = bufTmp.substr(bufTmp.find("PASS") + 4);
			bufTmp = trimSpace(bufTmp);
			_splitBuf[0] = "PASS";
			_splitBuf[1] = bufTmp.substr(0, server->getPassword().size());
			this->enterPwd(clients, server, fdClient);
			size_t k = bufTmp.find("NICK");
			if (k != std::string::npos)
				bufTmp = bufTmp.substr(bufTmp.find("NICK"));
		} 
		if (bufTmp.find("PASS ") == 0){
			std::cout << "ici2" << std::endl;

			bufTmp = bufTmp.substr(bufTmp.find("PASS") + 4);
			bufTmp = trimSpace(bufTmp);
			_splitBuf[0] = "PASS";
			_splitBuf[1] = bufTmp.substr(0, server->getPassword().size());
			this->enterPwd(clients, server, fdClient);
		}
		if (bufTmp.find("NICK ") == 0){

			std::cout << "ici3" << std::endl;

			_splitBuf[0] = "NICK";
			bufTmp = bufTmp.substr(bufTmp.find("NICK") + 4);
			bufTmp = trimSpace(bufTmp);
			std::cout  << bufTmp << std::endl;
			_splitBuf[1] = bufTmp.substr(0, bufTmp.find("\r"));
			size_t j = bufTmp.find("USER");
			if (j != std::string::npos)
				bufTmp = bufTmp.substr(bufTmp.find("USER"));
			this->setNick(clients, server, fdClient);
		}
		if (bufTmp.find("USER ") == 0){
			std::cout << "ici4" << std::endl;
			
			_splitBuf[0] = "USER";
			bufTmp = bufTmp.substr(bufTmp.find("USER") + 4);
			bufTmp = trimSpace(bufTmp);
			_splitBuf[1] = bufTmp.substr(0, bufTmp.find(" "));
			this->setName(clients, server, fdClient);
		}
}

void	Client::splitCmd( std::string cmdSend ){
	size_t j = cmdSend.find(" ");
	if (j != std::string::npos){
		this->_splitBuf.push_back(cmdSend.substr(0, j));
		cmdSend.erase(cmdSend.begin() , cmdSend.begin() + j);
		cmdSend = trimSpace(cmdSend);
		this->_splitBuf.push_back(cmdSend);
	}
	else{
		this->_splitBuf.push_back(cmdSend);
	}
}

void	Client::removeCmdBuf(){
 	this->_splitBuf.erase(this->_splitBuf.begin(), this->_splitBuf.end());
	_splitBuf.clear();
}
