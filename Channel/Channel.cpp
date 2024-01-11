#include "Channel.hpp"
#include <netinet/in.h>

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Channel::Channel( std::string name ) : _name(name)
{
	this->_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_socket == -1 )
		throw std::runtime_error( "Invalid socket." );

	this->_address.sin_addr.s_addr = INADDR_ANY;// toutes les sources acceptees.
	//this->_address.sin_port = htons( port );	// traduit le port en reseau.
	this->_address.sin_family = AF_INET;		// socket TCP IPv4.
										
	// le configurer et le mettre en attente:
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		close(this->_socket);
		throw std::runtime_error( "Can't bind to IP/port." );
	}

	if (listen(_socket, SOMAXCONN) < 0)
		throw std::runtime_error( "Can't listen, or too many clients to handle." );

	std::cout << "Channel " << this->_name << " created" << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Destructeur called" << std::endl;
}

fd_set	Channel::createFdSet( void )
{
	FD_ZERO(&this->_master);
	FD_SET(this->_socket, &this->_master);
	return(this->_master );
}

/* ************************************************************************** */
// ACCESSORS:
int	const & Channel::getSocket( void ) const{
	return( this->_socket );
}

std::string	const & Channel::getName( void ) const{
	return( this->_name);
}

sockaddr_in	const & Channel::getAddr( void ) const{
	return( this->_address );
}
