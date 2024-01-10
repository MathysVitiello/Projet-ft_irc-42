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
			// ERR_ERRONEUSNICKNAME
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
			if ( nick == it->getNickname() ){
				
				// return ERR_NICKNAMEINUSE;
			}
		this->_nickname = nick;
	}
	return ;
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

/* ************************************************************************** */
// FONCTIONS:

bool	Client::enterPwd(Server *server, std::string password){
	if (password == server->getPassword() && this->_connected == false){
		this->_connected = true;
		return true;
	}
	return false;
}

void    Client::privateMessage( std::vector<Client> *clients, std::string info )
{
	std::string name;
	// parse to find nick
	int i = 0;
	while (isspace(info[i]) != 0)
		i++;
	while (isspace(info[i]) == 0 || info[i] == ';')
	{
		name += info[i];
		i++;
	}
	name[i] = '\0';
	info = info.substr(i + 1);

	//find nickname dans tout les users
	std::vector<Client>::iterator it = clients->begin(); 
	for ( ;it < clients->end(); it++ )
	{
		if ( name == it->getNickname() ){
			
			// send message to the client
			send(it->getSocket(), "messade de ", 11, 0);
			const void * a = this->getNickname().c_str();
			send(it->getSocket(), a, this->getNickname().size(), 0);
			send(it->getSocket(), " : ", 3, 0);
			const void * b = info.c_str();
			send(it->getSocket(), b, info.size(), 0);
			send(it->getSocket(), "\n", 1, 0);
			return ;
		}
	}
	//! code d'erreur a dapater
	send(this->getSocket(), "Coudn't find Nickname : ", 24, 0);
	const void * c = name.c_str();
	send(this->getSocket(), c, name.size(), 0);
}