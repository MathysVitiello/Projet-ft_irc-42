#include "Channel.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Channel::Channel( Client *user, std::string name, std::string mdp) : _name(name),
																	   _password(mdp)
{
	std::cout << "Channel [" << this->_name << "] created" << std::endl;
	this->_user.push_back( *user );
	// std::cout << "Client admin: " << std::cout << user->getName();
}

Channel::~Channel( void )
{
	std::cout << "channel ["<< this->_name << "] Destructeur called" << std::endl;
}

/* ************************************************************************** */
// OPERATOR OVERLOAD:
Client *Channel::operator[](unsigned int index) {
	if ( index >= this->_user.size() )
		 throw std::runtime_error( "Index is invalid" );
	return &this->_user[index];
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

void	Channel::removeClientChannel( int index ){
	this->_user.erase( this->_user.begin() + index );
}
