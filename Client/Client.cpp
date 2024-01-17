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
	this->_nickname = "*";

	return ;
}

Client::~Client( void ) {

	//std::cout << "Client Destructor called" << std::endl;
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

std::vector<std::string>	const & Client::getCmdBuf(void) const{
	return this->_splitBuf;
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

void Client::setNick( Server *server ) {
	parsHexchat();
	if (this->_connected){
		if (_splitBuf[0].empty()){
			send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(),
					ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).size(), 0);
			removeCmdBuf();
			return;
		}
		if (_splitBuf[1].size() > 9){
			send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
					ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
			removeCmdBuf();
			return;
		}
		for(size_t i = 0; i <_splitBuf[1].size(); i++){
			if (!isalpha(_splitBuf[1][0])){
				std::cout << _splitBuf[1] << std::endl;
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
						ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
			if (!isalnum(_splitBuf[1][i]) && _splitBuf[1][i] != '-' && _splitBuf[1][i] != '[' && _splitBuf[1][i] != ']' 
					&& _splitBuf[1][i] != '\\' &&  _splitBuf[1][i] != '^' && _splitBuf[1][i] != '_' && _splitBuf[1][i] != '{' 
					&& _splitBuf[1][i] != '|' && _splitBuf[1][i] != '}'){
				send(this->getSocket(),	ERR_ERRONEUSNICKNAME(_splitBuf[1]).c_str(), 
						ERR_ERRONEUSNICKNAME(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
		}
		std::vector<Client>::const_iterator it = server->getClients().begin(); 
		for ( ;it < server->getClients().end(); it++ )
			if ( _splitBuf[1] == it->getNickname() ){
				send(this->getSocket(),ERR_NICKNAMEINUSE(_splitBuf[1]).c_str(),
						ERR_NICKNAMEINUSE(_splitBuf[1]).size(), 0);
				removeCmdBuf();
				return;
			}
		this->_nickname = _splitBuf[1];
		if (3 == _splitBuf.size()){
			_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 1);
			server->command(this->_socket);
		}
	}
	removeCmdBuf();
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

void	Client::enterPwd(Server *server){
	parsHexchat();
	// std::cout << " ++++++++" <<  _splitBuf[1] << std::endl;
	if (this->_connected == true){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
	}
	else if (this->_splitBuf[1].empty()){
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), 
				ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[1]).size(), 0);
	}
	else if (_splitBuf[1] == server->getPassword()){
		//	 std::cout << " -|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;

		this->_connected = true;
		if (3 == _splitBuf.size()){
			_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 1);
			// std::cout << " -------------------" << _splitBuf[0] << std::endl;
			//		std::cout << "Merde " << std::endl;	
			server->command(this->_socket);
		}
	}
	else{
		send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(),
				ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
	}
	// for (int i = 0; i < 2; i++)
	// _splitBuf[i].erase();
	removeCmdBuf();
}

void    Client::privateMessage( std::vector<Client> *clients, Server *server, std::string info, int fdClient)
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
	if (name[0] != '#' && name[0] != '&')
		send(this->getSocket(), ERR_NOSUCHNICK(this->_nickname, name).c_str(), ERR_NOSUCHNICK(this->_nickname, name).size(), 0);

	//! check si cest un channel
	std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
	for ( ; itChan < server->getChannels().end(); itChan++)
		if ( itChan->getName() == name )
		{
			// send message to the clients of the channel
			server->sendMessageChanel( fdClient, info ); //! pas le bon fd je crois. envoie dans le terminal et a sois
			return;
		}
	send(this->getSocket(), ERR_NOSUCHSERVER(name).c_str(), ERR_NOSUCHSERVER(name).size(), 0);


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
			server->createChannel(this->getSocket(), join, "");
			return;
		}  else {
			//find password
			std::string chanel = join.substr(0, join.find(' '));
			if (chanel[0] != '&')
				return;
			join = join.substr(join.find(' '));
			join = trimSpace(join);
			server->createChannel(this->getSocket(), chanel, join);
		}
	}
}

void	Client::splitCmd( std::string cmdSend ){
	std::cout << "Notre buf initial :" << cmdSend;
	// if (!_splitBuf.empty())
		// for (size_t i = 0; i <= _splitBuf.size(); i++)
			// std::cout << "Notre splitbuf initial :" <<  _splitBuf[i] << std::endl;
	if (cmdSend.find("CAP") != std::string::npos ){

		cmdSend.erase(0, 12);

	}
	size_t j = cmdSend.find(" ");
	if (j != std::string::npos){
		this->_splitBuf.push_back(cmdSend.substr(0, j));
		cmdSend.erase(cmdSend.begin() , cmdSend.begin() + j);
		cmdSend = trimSpace(cmdSend);
		this->_splitBuf.push_back(cmdSend);
		// std::cout << "ICI" <<  this->_splitBuf[0] << std::endl;
		// std::cout << "ou la "  << cmdSend << std::endl;
	}
	else
		this->_splitBuf.push_back(cmdSend);
	// std::cout << _splitBuf[0] << std::endl;
}

void	Client::removeCmdBuf(){
		// std::cout << "    REMOVE buff " << std::endl;
	for (size_t i = 0; i <= _splitBuf.size(); i++){
		std::cout << _splitBuf[i] << std::endl;
		_splitBuf.erase(_splitBuf.begin( ) + i);
}
		// std::cout << " END   REMOVE buff " << std::endl;
}

void	Client::parsHexchat( void ){
	size_t delem = _splitBuf[1].find("\r\n"); 
	if ( delem != std::string::npos){

		std::string tmp = _splitBuf[1];
		std::cout << std::endl <<  tmp << std::endl << std::endl << tmp.substr(0, delem) << std::endl << std::endl  << std::endl << std::endl  ;
		_splitBuf[1].erase();
		_splitBuf.push_back(tmp.substr(0, delem));
		_splitBuf.push_back(tmp.substr(delem + 1));
		// for (int i = 0; i < 3; i++)
		// std::cout << "ICI =" << _splitBuf[i] << "=" << std::endl;
	}
	else
		_splitBuf[1].erase(_splitBuf[1].size() - 1); //remove \n

	std::cout << " l'argument commande a ce moment " << _splitBuf[1];
}

