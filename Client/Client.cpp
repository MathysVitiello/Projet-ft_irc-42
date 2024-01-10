#include "Client.hpp"


/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Client::Client( int const & id, sockaddr_in from ) :	_id( id ),
	_address( from ),
	_connected(false)
{
	std::cout << "Client constructor called" << std::endl;
	std::cout << " id:  " << this->_id << std::endl;
	std::cout << " reseau :  " << this->_address.sin_port << std::endl;
	this->_name = "";
	this->_nickName = "";

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
int	const & Client::getId( void ) const
{
	return( this->_id );
}

sockaddr_in const & Client::getAddr( void ) const
{
	return( this->_address );
}

std::string const & Client::getName( void ) const
{
	return( this->_name );
}

std::string const & Client::getNickName( void ) const
{
	return( this->_nickName );
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
	if (this->_connected || !this->_nickName.empty() || this->_nickName.size() < 9){
		// if (nick non valid char)
			// ERR_ERRONEUSNICKNAME
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
			if ( nick == it->getNickName() ){
				
				// return ERR_NICKNAMEINUSE;
			}
		this->_nickName = nick;
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
