#include "Client.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Client::Client( int const & id, sockaddr_in from ) :	_socket( id ),
														_address( from ),
														_connected(false),
														_checkRight(false){
	std::cout << "Client constructor called" << std::endl;
	std::cout << " id:  " << this->_socket << std::endl;
	std::cout << " reseau :  " << this->_address.sin_port << std::endl;
	this->_name = "";
	this->_nickname = "*";
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
	size_t delem = _splitBuf[1].find("\r\n"); 
	if ( delem != std::string::npos){

		std::string tmp = _splitBuf[1];
		std::cout << std::endl <<  tmp << std::endl << std::endl << tmp.substr(0, delem) << std::endl << std::endl  << std::endl << std::endl  ;
		_splitBuf[1].erase();
		_splitBuf.push_back(tmp.substr(0, delem));
		_splitBuf.push_back(tmp.substr(delem + 1));
		for (int i = 0; i < 3; i++)
			std::cout << "ICI =" << _splitBuf[i] << "=" << std::endl;
	}
	else
		_splitBuf[1].erase(_splitBuf[1].size() - 1);
	std::cout << " ++++++++" <<  _splitBuf[1] << std::endl;
	if (this->_connected == true){
		send(this->getSocket(), ERR_ALREADYREGISTERED(this->_nickname).c_str(),
				ERR_ALREADYREGISTERED(this->_nickname).size(), 0);
	}
	else if (this->_splitBuf[1].empty()){
		send(this->getSocket(), ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[0]).c_str(), 
				ERR_NEEDMOREPARAMS(this->_nickname, _splitBuf[1]).size(), 0);
	}
	else if (_splitBuf[1] == server->getPassword()){
			// std::cout << " -------------------" << std::endl;

		this->_connected = true;
		if (3 == _splitBuf.size()){
			_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 1);
			// std::cout << " -------------------" << _splitBuf[0] << std::endl;
			std::cout << "Merde " << std::endl;	
			server->command(this->_socket);
		}
	}
	else{
		send(this->getSocket(), ERR_PASSWDMISMATCH(this->_nickname).c_str(),
			ERR_PASSWDMISMATCH(this->_nickname).size(), 0);
	}
	// for (int i = 0; i < 2; i++)
		// _splitBuf[i].erase();
	_splitBuf.clear();
	for (int i = 0; i < 2; i++)
			std::cout << "33333333333=" << _splitBuf[i] << "=" << std::endl;
}

void    Client::privateMessage( std::vector<Client> *clients, Server *server, int fdClient)
{
//! CETTE FUNCTION FONCTIONNE AVEC NC, SANS TEST AVEC D'AUTRES USERS
	
	if (_splitBuf[1].find(" ") < _splitBuf[1].find("\n"))
	{
		std::string nickOrChannel = _splitBuf[1].substr(0, _splitBuf[1].find(" "));
		std::cout << "nickOrChannel :" << nickOrChannel << "." << std::endl; 
		//find nickname dans tout les users
		std::vector<Client>::iterator it = clients->begin(); 
		for ( ;it < clients->end(); it++ )
		{
			if ( nickOrChannel == it->getNickname() ){
				// send message to the client
				if (_splitBuf[1].substr(_splitBuf[1].find(" ")).size() == 0)
					send(this->getSocket(), ERR_NOTEXTTOSEND(this->getNickname()).c_str(), ERR_NOTEXTTOSEND(this->getNickname()).size(), 0);
				else 
					send(it->getSocket(), RPL_AWAY(nickOrChannel , _splitBuf[1].substr(_splitBuf[1].find(" "))).c_str(),
						RPL_AWAY(nickOrChannel, _splitBuf[1].substr(_splitBuf[1].find(" "))).size(), 0);
				return ;
			}
		}
		if (nickOrChannel[0] != '#' && nickOrChannel[0] != '&')
			send(this->getSocket(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).c_str(), ERR_NOSUCHNICK(this->_nickname, nickOrChannel).size(), 0);

		// check si cest un channel
		std::vector<Channel>::const_iterator itChan = server->getChannels().begin();
		for ( ; itChan < server->getChannels().end(); itChan++)
			if ( itChan->getName() == nickOrChannel )
			{
				// send message to the clients of the channel
				server->sendMessageChanel( fdClient, _splitBuf[1].substr(_splitBuf[1].find(" "))); //! pas le bon fd je crois. envoie dans le terminal et a sois				_splitBuf.erase (_splitBuf.begin()+1); // efface pour la prochaine commande
				return;
			}
		send(this->getSocket(), ERR_NOSUCHSERVER(nickOrChannel).c_str(), ERR_NOSUCHSERVER(nickOrChannel).size(), 0);
		//bon nombre d'args
	} else {
		send(this->getSocket(), ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").c_str(), 
				ERR_NORECIPIENT(this->getNickname(), "PRIVMSG").size(), 0);
		return ;
	}
}

//! test apres merge si JOIN sans espace core dump, meme  pour privmsg
void	Client::join( Server *server )
{
	std::cout << "size :" << _splitBuf[1].size() << std::endl;
    if (_splitBuf[1].empty()){ //! ne focntionone pas, voir avec NIL
        send(this->getSocket(), ERR_NEEDMOREPARAMS(this->getName(), "JOIN").c_str(),
            ERR_NEEDMOREPARAMS(this->getName(), "JOIN").size(), 0);
        return;
    }
    if (_splitBuf[1][0] == '#' || _splitBuf[1][0] == '&')
    {
        if (_splitBuf[1].size() <= _splitBuf[1].find(' '))
        {
            //no password
            if (_splitBuf[1][0] != '#')
                return;
            server->createChannel(this->getSocket(), _splitBuf[1], "");
			std::cout << "sans un password :"  << _splitBuf[1] << std::endl;
            return;
        }  else {
            //find password
            std::string chanel = _splitBuf[1].substr(0, _splitBuf[1].find(' '));           
		    if (chanel[0] != '&')
                return;
            _splitBuf[1] = _splitBuf[1].substr(_splitBuf[1].find(' '));
            _splitBuf[1] = trimSpace(_splitBuf[1]);
			//! ici si il y a un espace, pas bon
			//! sassure que il t a un mdpppp
			if (_splitBuf[1].find(" ") < _splitBuf[1].find("\n"))
			{
				std::cout << "il y a trop d'args apres le mdp" << std::endl;
				return;
			}
            server->createChannel(this->getSocket(), chanel, _splitBuf[1]);
        }
    }
	//! gerer hex cat si il a \r\n
}

void	Client::splitCmd( std::string cmdSend ){
	// size_t checkR = cmdSend.find("\n");
	// if (cmdSend[checkR - 1] != '\r')
	// 	cmdSend.insert(checkR - 1, "\r");

	if (cmdSend.find("CAP") != std::string::npos ){

		cmdSend.erase(0, 12);
		// std::cout << " ici  hexchat: " << cmdSend << std::endl;

	}
	// for (unsigned int i = 0; i < cmdSend.size(); i++){
		size_t j = cmdSend.find(" ");
	 if (j != std::string::npos){
		// 	this->_splitBuf.push_back(cmdSend.substr(0));
		// 	cmdSend.erase(cmdSend.begin(), cmdSend.end());
		// 	return;
		// }
		// size_t k = cmdSend.find("\r\n");
		// if ( k != std::string::npos && k < j){
		
		// 	this->_splitBuf.push_back(cmdSend.substr(0, k));
		// 	cmdSend.erase(cmdSend.begin(), cmdSend.begin() + k);
			// std::cout << " ---  " << cmdSend;

			// this->_splitBuf.push_back(cmdSend.substr(0, 2));
			// cmdSend.erase(cmdSend.begin(), cmdSend.begin() + 2);
			// std::cout << " +++  " << cmdSend;

		this->_splitBuf.push_back(cmdSend.substr(0, j));
	 	// std::cout  << j << std::endl;
		cmdSend.erase(cmdSend.begin() , cmdSend.begin() + j);
	 	// std::cout  << j << std::endl;
		cmdSend = trimSpace(cmdSend);
		this->_splitBuf.push_back(cmdSend);
	 	//std::cout  << "==============================-" << std::endl;
		// }
		// else {
		// 	this->_splitBuf.push_back(cmdSend.substr(0, j));
		// cmdSend.erase(cmdSend.begin() , cmdSend.begin() + j);
		// 	cmdSend = trimSpace(cmdSend);
		// }
	}
	else
		this->_splitBuf.push_back(cmdSend);

	
	// if (_splitBuf[0] == "CAP"){
	// 	_splitBuf.erase(_splitBuf.begin(), _splitBuf.begin() + 3	);
	// }
	//std::cout << _splitBuf[0] << std::endl;
}


void    Client::removeCmdBuf(){
    for (int i = 0; i < 2; i++)
        _splitBuf.erase(_splitBuf.begin( ) + i);
}