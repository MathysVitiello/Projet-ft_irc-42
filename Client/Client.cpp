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

bool	const & Client::getInCanal( void ) const{
	return( this->_inCanal );
}

void	Client::setSocket( int socket ){
	this->_socket = socket;
}

void Client::setName( std::string name ) {
	name = trimSpace(name);
	if (this->_connected){
		if (name.empty()){
			std::string cmd = "USER";
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(),
					ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
			return;
		}
		if (this->_name.empty()){
			for(size_t i = 0; i < name.size(); i++){
				if (!isalnum(name[i]) && name[i] != '-' && name[i] != '[' && name[i] != ']' 
						&& name[i] != '\\' &&  name[i] != '^' && name[i] != '_' 
						&& name[i] != '{' && name[i] != '|' && name[i] != '}'){
					send(this->getSocket(),	ERR_ERRONEUSNICKNAME(name).c_str(), 
							ERR_ERRONEUSNICKNAME(name).size(), 0);
					return;
				}
			}
			this->_name = name;
			return;
		}
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_name).c_str(),
				ERR_ALREADYREGISTERED(this->_name).size(), 0);
	}
	return ;
}

void Client::setNick( std::string nick, std::vector<Client> *clients ) {
	nick  = trimSpace(nick);
	if (this->_connected){
		if (nick.empty()){
			std::string cmd = "NICK";
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(), 
					ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
			return;
		}
		if (nick.size() > 9){
			send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), 
					ERR_ERRONEUSNICKNAME(nick).size(), 0);
			return;
		}
		for(size_t i = 0; i < nick.size(); i++){
			if (!isalpha(nick[0])){
				std::cout << nick << std::endl;
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), 
						ERR_ERRONEUSNICKNAME(nick).size(), 0);
				return;
			}
			if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' && nick[i] != ']' 
					&& nick[i] != '\\' &&  nick[i] != '^' && nick[i] != '_' && nick[i] != '{' 
					&& nick[i] != '|' && nick[i] != '}'){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(nick).c_str(), 
						ERR_ERRONEUSNICKNAME(nick).size(), 0);
				return;
			}
		}
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
			if ( nick == it->getNickname() ){
				send(this->getSocket(),ERR_NICKNAMEINUSE(nick).c_str(),
						ERR_NICKNAMEINUSE(nick).size(), 0);
				return;
			}
		this->_nickname = nick;
	}
	return ;
}

void	Client::setInCanal( bool foo ){
	this->_inCanal = foo;
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

void	Client::enterPwd(Server *server, std::string password){
	if (password[0] == ' ')
		password.erase(0, 1);
	if (this->_connected == true){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
		return;
	}
	if (password.empty()){
		std::string	cmd = "PASS";
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, cmd).c_str(), 
				ERR_NEEDMOREPARAMS(this->_nickname, cmd).size(), 0);
		return;
	}
	if (password == server->getPassword()){
		this->_connected = true;
		return;
	}
	send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(),
			ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
}

void    Client::privateMessage( std::vector<Client> *clients, std::string info )
{
	int i = 0;
	std::string name;

	info = trimSpace(info);
	if (info.size() == 0)
	{
		send(this->getSocket(), ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").c_str(), 
				ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").size(), 0);
		return ;
	}
	while (info[i] && isspace(info[i]) == 0)
	{
		name += info[i];
		i++;
	}
	name[i] = '\0';
	info = info.substr(i);
	info = trimSpace(info);

	//find nickname dans tout les users
	std::vector<Client>::iterator it = clients->begin(); 
	for ( ;it < clients->end(); it++ )
	{
		if ( name == it->getNickname() ){
			
			// send message to the client
			if (info.size() == 0)
				send(this->getSocket(), ERR_NOTEXTTOSEND(this->getNickname()).c_str(), ERR_NOTEXTTOSEND(this->getNickname()).size(), 0);
			else 
				send(it->getSocket(), RPL_AWAY(name , info).c_str(), RPL_AWAY(name, info).size(), 0);
			return ;
		}
	}
	send(this->getSocket(), ERR_NOSUCHNICK(name).c_str(), ERR_NOSUCHNICK(name).size(), 0);
}

void	Client::join(Server *server, std::string join )
{
    join = trimSpace(join);
    if (join.empty()){
        send(this->getSocket(), ERR_NEEDMOREPARAMS(this->getName(), "JOIN").c_str(),
            ERR_NEEDMOREPARAMS(this->getName(), "JOIN").size(), 0);
        return;
    }
    if (join[0] == '#' || join[0] == '&')
    {
        if (join.size() <= join.find(' '))
        {
            //no password
            if (join[0] != '#')
                return;
            server->createChannel(this, join, "");
            return;
        }  else {
            //find password
            std::string chanel = join.substr(0, join.find(' '));
            if (chanel[0] != '&')
                return;
            join = join.substr(join.find(' '));
            join = trimSpace(join);
            server->createChannel(this, chanel, join);
        }
    }
}


