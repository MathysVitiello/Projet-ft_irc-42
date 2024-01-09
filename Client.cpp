#include "library.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Client::Client( int const & id, sockaddr_in from ) :	_id( id ),
														_address( from ),
														_connected(false)
{
    std::cout << "Client constructor called" << std::endl;
	std::cout << " id:  " << this->_id << std::endl;
	std::cout << " reseau :  " << this->_address.sin_port << std::endl;
	this->_name = "nobody";
	this->_nickName = "bobo";

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

    this->_name = name;
    return ;
}

void Client::setNick( std::string nick ) {

    this->_nickName = nick;
    return ;
}


void Client::setAddr( sockaddr_in addr ) {

    this->_address = addr;
    return ;
}

void	Client::setConnect( bool password ) {
	this->_connected = password;
}
