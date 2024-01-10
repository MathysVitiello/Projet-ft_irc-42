#include "Client.hpp"


/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Client::Client( int const & id, sockaddr_in from ) :	_socket( id ),
	_address( from ),
	_connected(false)
{
	std::cout << "Client constructor called" << std::endl;
	std::cout << " id:  " << this->_socket << std::endl;
	std::cout << " reseau :  " << this->_address.sin_port << std::endl;
	this->_name = "";
	this->_nickname = "";

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
int	const & Client::getSocket( void ) const
{
	return( this->_socket );
}

sockaddr_in const & Client::getAddr( void ) const
{
	return( this->_address );
}

std::string const & Client::getName( void ) const
{
	return( this->_name );
}

std::string const & Client::getNickname( void ) const
{
	return( this->_nickname );
}

bool	const & Client::getConnect( void ) const{
	return( this->_connected );
}

void Client::setName( std::string name ) {
	if (this->_connected)
		if(this->_name.empty())
			this->_name = name;
	return ;
}

void Client::setNick( std::string nick, std::vector<Client> *clients ) {
	if (this->_connected || !this->_nickname.empty() || this->_nickname.size() < 9){
		// if (nick non valid char)
			// return ERR_ERRONEUSNICKNAME
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
			if ( nick == it->getNickname() ){
				send(this->getSocket(),ERR_NICKNAMEINUSE(nick).c_str() , ERR_NICKNAMEINUSE(nick).size(), 0);
				return;
			}
		this->_nickname = nick;
	}
	return ;
}


void Client::setAddr( sockaddr_in addr ) {

	this->_address = addr;
	return ;
}

/* ************************************************************************** */
// FONCTIONS:

bool	Client::enterPwd(Server *server, std::string password){
	if (password == server->getPassword() && this->_connected == false){
		this->_connected = true;
		return true;
	}
	return false;
}
