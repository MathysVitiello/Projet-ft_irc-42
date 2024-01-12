#include "Channel.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Channel::Channel( Client *user, std::string name, std::string mdp) : _name(name),
																	   _password(mdp)
{
	std::cout << "Channel [" << this->_name << "] created" << std::endl;
	this->_user.push_back( *user );
}

Channel::~Channel( void )
{
	std::cout << "channel ["<< this->_name << "] Destructeur called" << std::endl;
}

/* ************************************************************************** */
// ACCESSORS:
std::string	const & Channel::getName( void ) const{
	return( this->_name);
}

std::vector<Client>	const & Channel::getUser( void ) const{
	return( this->_user);
}

std::string	const & Channel::getPasswd( void ) const{
	return( this->_password );
}

