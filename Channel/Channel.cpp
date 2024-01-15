#include "Channel.hpp"
#include <sys/select.h>

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Channel::Channel( int userSocket, std::string name, std::string mdp) :
	_name( name ),
	_password( mdp ),
	_owner( userSocket ),
	_maxUser( FD_SETSIZE ){
	// Verifier en amont si le canal existe deja et que le client n'y est pas.
	std::cout << "Channel [" << this->_name << "] created" "Owner socket: " << userSocket << std::endl;

	this->_ircOps.push_back( userSocket );
	this->_user.push_back( userSocket );
}

Channel::~Channel( void ){
	// std::cout << "channel ["<< this->_name << "] closed" << std::endl;

	if ( this->_user.empty() )
		this->_user.erase( this->_user.begin(), this->_user.end() );

	if ( this->_ircOps.empty() )
		this->_user.erase( this->_user.begin(), this->_user.end() );

	this->_ircOps.clear();
	this->_user.clear();
}

/* ************************************************************************** */
// OPERATOR OVERLOAD:
int Channel::operator[](unsigned int index) {
	if ( index >= this->_user.size() )
		 throw std::runtime_error( "Index is invalid" );
	return this->_user[index];
}

/* ************************************************************************** */
// ACCESSORS:
std::string	const & Channel::getName( void ) const{
	return( this->_name);
}

std::string	const & Channel::getPasswd( void ) const{
	return( this->_password );
}

int const &	Channel::getOwner( void ) const{
	return( this->_owner );
}

std::vector<int>	const & Channel::getIrcOps( void ) const{
	return( this->_user);
}

std::vector<int>	const & Channel::getUser( void ) const{
	return( this->_user);
}

// void		setUSER( int index , std::string name ){
// }

/* ************************************************************************** */
// FUNCTIONS:
void	Channel::addClientChannel( int clientSocket ){
	if( static_cast<unsigned>(this->_maxUser) >= this->_user.size() )
		this->_user.push_back( clientSocket );
	else
		std::cout << "No more user" << std::endl;
}

void	Channel::removeClientChannel( int userSocket ){
	//Supprime l'ircOps du canal:
	for(std::vector<int>::iterator it = this->_ircOps.begin(); it != this->_ircOps.end(); it++)
	{
		if( *it == userSocket )
		{
			this->_ircOps.erase( it );
			std::cout << "IrcOps socket [" << userSocket << "] erased" << std::endl;
			// pensez a close les clients respectifs
		}
	}

	// Supprime le user du canal:
	for(std::vector<int>::iterator it = this->_user.begin(); it != this->_user.end(); it++)
	{
		if( *it == userSocket )
		{
			this->_user.erase( it );
			std::cout << "User socket [" << userSocket << "] erased" << std::endl;
		}
	}
}
