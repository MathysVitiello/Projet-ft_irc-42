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

std::vector<std::string>	const & Client::getCmdBuf(void) const{
	return this->_splitBuf;
}

std::string		Client::getBufTmp( void ){
	return this->bufTmp;
}

void	Client::setSocket( int socket ){
	this->_socket = socket;
}

int	Client::checkRight( void ) {
	if (this->_connected == true && this->getName() != "" && this->getNickname() != "")
		return true;
	return false;
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

void	Client::capForHex( Server *server, int fdClient, std::vector<Client> *clients){

		if (_splitBuf[1].size() > 11)
		{
			_splitBuf[0] = "PASS";
			_splitBuf[1] = _splitBuf[1].substr(12);
			this[fdClient].enterPwd(clients, server, fdClient);
		} else {
			//only CAP line in hexchat
			_splitBuf.clear();
		}
		}

void	Client::splitCmd( std::string cmdSend ){
	if (cmdSend.find("CAP") != std::string::npos ){
		cmdSend.erase(0, 12);
	}
	size_t j = cmdSend.find(" ");
	if (j != std::string::npos){
		this->_splitBuf.push_back(cmdSend.substr(0, j));
		cmdSend.erase(cmdSend.begin() , cmdSend.begin() + j);
		cmdSend = trimSpace(cmdSend);
		this->_splitBuf.push_back(cmdSend);
	}
	else
		this->_splitBuf.push_back(cmdSend);
}

void	Client::removeCmdBuf(){

 	this->_splitBuf.erase(this->_splitBuf.begin(), this->_splitBuf.end());
	this->_splitBuf.clear();
}

void	Client::parsHexchat( void ){

	size_t delem = _splitBuf[1].find("\r"); 
	if ( delem != std::string::npos){

		std::string tmp = _splitBuf[1];
		_splitBuf[1].erase();
		_splitBuf.push_back(tmp.substr(0, delem));
		_splitBuf.push_back(tmp.substr(delem + 1));
	}
}

