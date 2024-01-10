#include "Client.hpp"
#include <cstdio>
#include <cstring>
#include <string>


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
	if (name[0] == ' ')
		name.erase(0, 1);
	if (this->_connected){
		if (name.empty()){
			std::string cmd = "USER";
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
			return;
		}
		if (this->_name.empty()){
			this->_name = name;
			return;
		}
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_name).c_str(),ERR_ALREADYREGISTERED(this->_name).size(), 0);
	}
	return ;
}

void Client::setNick( std::string nick, std::vector<Client> *clients ) {
	if (nick[0] == ' ')
		nick.erase(0, 1);
	if (this->_connected){
		if (nick.empty()){
			std::string cmd = "NICK";
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
		}
		if (this->_nickname.size() > 9){
			send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), ERR_ERRONEUSNICKNAME(nick).size(), 0);
			return;
		}
		for(size_t i = 0; i < nick.size(); i++){
			if (!isalpha(nick[0])){
				std::cout << nick << std::endl;
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), ERR_ERRONEUSNICKNAME(nick).size(), 0);
				return;
			}
			if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' && nick[i] != ']' && nick[i] != '\\' &&  nick[i] != '^' && nick[i] != '_' && nick[i] != '{' && nick[i] != '|' && nick[i] != '}'){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), ERR_ERRONEUSNICKNAME(nick).size(), 0);
				return;
			}
		}
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

void	Client::enterPwd(Server *server, std::string password){
	if (password[0] == ' ')
		password.erase(0, 1);
	if (this->_connected == true){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
		return;
	}
	if (password.empty()){
		std::string	cmd = "PASS";
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
		return;
	}
	if (password == server->getPassword()){
		this->_connected = true;
		return;
	}
	send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(), ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
}
