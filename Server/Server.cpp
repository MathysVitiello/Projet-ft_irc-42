#include "Server.hpp"
#include <string>
#include <vector>
/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Server::Server( unsigned int const & port, std::string const & password  ): _port(port),
																		 	_password(password)
{
	this->_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_socket == -1 )
		throw std::runtime_error( "Don't opening socket." );

	this->_address.sin_addr.s_addr = INADDR_ANY;// accepeted sources
	this->_address.sin_port = htons( port );	// traduit le port en reseau.
	this->_address.sin_family = AF_INET;		// socket TCP IPv4.
										
	// le configurer et le mettre en attente:
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		close(this->_socket);
		throw std::runtime_error( "Can't bind to IP/port." );
	}

	if (listen(_socket, SOMAXCONN) < 0)
		throw std::runtime_error( "Can't listen, or too many clients to handle." );

	std::cout << BGREEN << "###############################################" << std::endl;
	std::cout << "SERVER STARTING..." << std::endl;
	std::cout << " -Memory Adress: " << this << std::endl;
	std::cout << "###############################################" << NC << std::endl;
	return;
}

Server::~Server( void )
{
	close( this->_socket );

	for(size_t i = 0; i < this->getClients().size(); i++)
	{
		if( this->getClients()[i].getSocket() > 0 )
		{
			int socket = this->getClients()[i].getSocket();
			close( socket );
		}
	}
	return;
}

/* ************************************************************************** */
// OPERATOR OVERLOAD:
Client *Server::operator[](unsigned int index) {
	if ( index >= this->_clients.size() )
		 throw std::runtime_error( "Index is invalid" );
	return &this->_clients[index];
}

/* ************************************************************************** */
// ---------------------- GETTERS -------------------- //
unsigned int const & Server::getPort() const{
	return( this->_port );
}

int	const & Server::getSocket() const{
	return( this->_socket );
}

std::string const & Server::getPassword() const{
	return( this->_password );
}
sockaddr_in	const & Server::getAddr() const{
	return( this->_address );
}

std::vector<Client> const & Server::getClients( void ) const{
	return( this->_clients );
}

std::vector<Channel> const & Server::getChannels( void ) const{
	return( this->_channels );
}

bool	Server::getInCanal( Server * server, int fdClient ){

	std::vector<Channel>::iterator it;
	for(it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		for(size_t i = 0; i < it->getUser().size(); i++)
		{
			if( server->_clients[fdClient].getSocket() == it->getUser()[i] )
				return true;
		}
	}
	return false;
}

void	Server::setClients( std::string buf, int index ){
	this->_clients[index].splitCmd( buf );
	return;
}

/* ************************************************************************** */
// FUNCTIONS:
// Rajoute un client dans le tableau de clients du serveur:
void	Server::addClient( int const & id, sockaddr_in addr)
{
	// max client 1024-> Fd_SETSIZE:
	Client client(id, addr);
	this->_clients.push_back( client );
}

// Supprime le channel si le client est Owner:
void	Server::eraseOwnerChannel( int socket )
{
	int i = 0;
	std::vector<Channel>::iterator it;
	for(it = this->_channels.begin(); it != this->_channels.end(); it++, i++)
		if( socket == it->getOwner() )
			break;	
	std::cout << this->_channels[i].getName() << std::endl;
	this->_channels.erase( this->_channels.begin() + i );
}

// Deletes clients from all channels
void	Server::removeClient( int const & index )
{
	int socketClient = this->_clients[index].getSocket();
	std::string nick = this->_clients[index].getNickname();

	std::vector<Channel>::iterator it;
	if (  !this->_channels.empty() )
	{
		for(it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			if( socketClient == it->getOwner() )
			{
				this->eraseOwnerChannel( socketClient );
				it--;
			}
			else{
				for(size_t i = 0; i < it->getUser().size(); i++)
					if( socketClient == it->getUser()[i])
						it->removeClientChannel( socketClient );
			}
		}
	}
	close( socketClient );
	this->_clients.erase( this->_clients.begin() + index );
}

void	Server::kickUser( int socketToKick, std::string channelName, std::string message){

		std::vector<Channel>::iterator itChan;
		for(itChan = this->_channels.begin(); itChan != this->_channels.end(); itChan++)
		{
			if ( itChan->getName() == channelName )
			{
				itChan->removeClientChannel( socketToKick );
				if (message.size() != 0)
					send(socketToKick, KICK_MESSAGE(channelName, message).c_str(), KICK_MESSAGE(channelName, message).size(), 0);
				else
					send(socketToKick, KICK_NOMESSAGE(channelName).c_str(), KICK_NOMESSAGE(channelName).size(), 0);
			}
		}
}

void	Server::command(int fdClient){

	std::string	cmd[] = {"CAP", "PASS", "NICK", "USER", "PRIVMSG", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART"};
	int i;

	for (i = 0; i < 11; i++){
		if(this->_clients[fdClient].getCmdBuf().empty()){
			i = -1; 
			break;
		}
			if(this->_clients[fdClient].getCmdBuf()[0] == cmd[i])
				break;
	}

		switch (i) {
		case CAP:
			std::cout << "CAP in switch case " << std::endl;
			this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);
			//this->_clients[fdClient].enterPwd(&this->_clients, this, fdClient);
			break;
		case PASS:
			std::cout << "PASS in switch case " << std::endl;
			this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);
			// ajout ici de remove, jai enlever dans la fiinction pour une raison
			break;
		case NICK:
			std::cout << "NICK in switch case " << this->_clients[fdClient].getCmdBuf()[1] << std::endl;
			this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);
			break;
		case USER:
			std::cout << "USER in switch case " << std::endl;
			this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);
			break;
		case PRIVMSG:
			std::cout << "PRIVMSG in switch case " << std::endl;
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].privateMessage(&this->_clients, this, fdClient);
			break;
		case JOIN:
			std::cout << "JOIN in switch case " << std::endl;
			if (this->_clients[fdClient].getConnectServer() == true){
				this->_clients[fdClient].join(this);
			}
			break;
		case KICK:
			std::cout << "KICK in switch case " << std::endl;
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].kick(this);
			break;
		case INVITE:
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].invitation( this );
			std::cout << "INVITE in switch case  " << std::endl;
			break;
		case TOPIC:
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].topic( this );
			std::cout << "TOPIC in switch case " << std::endl;
			break;
		case MODE:
			std::cout << "MODE in switch case " << std::endl;
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].mode(this);
			break;
		case PART:
			std::cout << "PART in switch case " << std::endl;
			if (this->_clients[fdClient].getConnectServer() == true)
				this->_clients[fdClient].part(this);
			break;
		default:
			send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
			break;
		}
		this->_clients[fdClient].removeCmdBuf();
	}

	// Verify if channel exists
	bool Server::checkChannel( std::string name ){
		for(unsigned int i = 0; i  < this->_channels.size(); i++ )
		{
			if( this->getChannels()[i].getName() == name )
			{
				//! stdError a faire, et english 
				std::cout << "Channel [" << name << "] already exists" << std::endl;
				return( true);
			}
		}
		return( false );
	}

	// Verify if client is in the channel
	bool	Server::userInChannel( int i, int clientSocket )
	{
		std::vector<int>::const_iterator ite = this->getChannels()[i].getUser().begin();
		for (; ite != this->getChannels()[i].getUser().end(); ite++)
		{
			if( *ite == clientSocket )
			{
				char buff[4096] = "client already in channel\r\n";
				send(clientSocket, buff, strlen(buff), 0);
				return( false );
			}
		}
		return( true );
	}

	// if channel is full
	void	Server::channelFull( int clientSocket )
	{
		std::vector<Client>::iterator it;
		for( it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if( it->getSocket() == clientSocket )
			{
				std::string nick = it->getNickname();
				std::string name = it->getName();
				send(it->getSocket(), ERR_CHANNELISFULL(nick, name).c_str(),
						ERR_CHANNELISFULL(nick, name).size(), 0);
			}
		}	
	}

void	Server::createChannel( Client client, std::string name, std::string passwd )
{
	int clientSocket = client.getSocket();
	int socketNewUser = -1;
	if( this->checkChannel( name ) == true )
	{
		for(unsigned int i = 0; i  < this->_channels.size(); i++ )
		{
			if( this->getChannels()[i].getName() == name )
			{
				if( !this->userInChannel( i, clientSocket ) )
					return;

				// if client is not in channel
				if( this->_channels[i].addClientChannel(clientSocket) == true )
				{
					// send(clientSocket, "Client added in Channel\r\n", 
						// strlen("Client added in Channel\r\n"), 0);

					this->allClient(&this->_channels[i], client);

					//! send pour les autres du channel sauf lui
					int nbChannel = 0;
					// check si le channel existe
					for( size_t i = 0; i < this->getChannels().size(); i++ ){

						if (this->getChannels()[i].getName() == name)
						{
							nbChannel = i;
							// !ERROR CODE
							std::cout << this->getChannels()[i].getName() << " est le channel" << std::endl;
						}
					}

					if (this->getChannels()[nbChannel].getUser().size() > 1)
					{
						for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ )
							if (clientSocket == this->_clients[i].getSocket())
								socketNewUser = i;

						for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ ){
							//send to all people from channel message that someone new joined
							if (clientSocket != this->getChannels()[nbChannel].getUser()[i])
							{
								send(this->getChannels()[nbChannel].getUser()[i], RPL_CHAN(this->_clients[socketNewUser].getNickname(), "JOIN", this->getChannels()[nbChannel].getName()).c_str(),
									RPL_CHAN(this->_clients[socketNewUser].getNickname(), "JOIN", this->getChannels()[nbChannel].getName()).size(), 0);
							}
						}
					}
				}
				else
					this->channelFull( clientSocket );
			}
		}
	}
	
	else	
	{
		Channel channel( clientSocket, name, passwd );
		this->_channels.push_back( channel );
		this->allClient(&channel, client);
	}
}

//!EN TRAVAUX
// void    Server::sendMessageChanel( std::string nickOrChannel, int clientPlace, std::string cmdSend, int socket)
// {
// 	int nbChannel = 0;
// 	// check si le channel existe
//     for( size_t i = 0; i < this->getChannels().size(); i++ ){

// 		if (this->getChannels()[i].getName() == nickOrChannel)
// 		{
// 			send(clientSocket, CHANNELMADE(name).c_str(), CHANNELMADE(name).size(), 0);
// 			std::cout << "Ajout du channel [" << name << "]" << std::endl;
// 			Channel channel( clientSocket, name, passwd );
// 			this->_channels.push_back( channel );
// 		}
// 	}
//}
	//!EN TRAVAUX
void    Server::sendMessageChanel( std::string nickOrChannel, int clientPlace, std::string cmdSend, int socket)
{
	int nbChannel = 0;
	// check si le channel existe
	for( size_t i = 0; i < this->getChannels().size(); i++ ){

		if (this->getChannels()[i].getName() == nickOrChannel)
		{
			nbChannel = i;
			//!ERROR CODE
			std::cout << this->getChannels()[i].getName() << " est le channel" << std::endl;
		}
	}

	// check si le client est dans le channel 
	bool clientInChannel = false;
	for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ ){
		if (socket == this->getChannels()[nbChannel].getUser()[i])
			clientInChannel = true;
	}
	if (clientInChannel == false){
			//!ERROR CODE
		std::cout << "le client nest pas dans le channel dans lequel il veut PRIVMSG" << std::endl;
		return;
	}

	if (this->getChannels()[nbChannel].getUser().size() > 1)
	{
		for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ ){

			std::cout << "les users du channel " << this->getChannels()[nbChannel].getUser()[i] << std::endl;
			std::cout << "socket " << socket << std::endl;
			std::cout << "this client socket " << this->getClients()[clientPlace].getSocket() << std::endl;
			if (socket != this->getChannels()[nbChannel].getUser()[i])
			{
				send(this->getClients()[i].getSocket(), RPL_TOPIC(this->getClients()[clientPlace].getNickname(), nickOrChannel, cmdSend).c_str(),
					RPL_TOPIC(this->getClients()[clientPlace].getNickname(), nickOrChannel, cmdSend).size(), 0);
				send(this->getClients()[i].getSocket(), "\n", 1, 0);
			}
		}
	}
	return ;
} 

	// Permet l affichage de toutes les donnees inclut dans le serveur:
	// - std::cout << server << std::endl;
	std::ostream & operator<<( std::ostream & o, Server const & src )
	{
		std::vector<Client>::const_iterator it;

		std::cout << BPURPLE << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		o << "Server port: " << src.getPort() << std::endl;
		o << "Server reseau: " << src.getAddr().sin_port<< std::endl;
		o << "Server password: " << src.getPassword() << std::endl;
		o << "      ------------" << std::endl;

		// Affiche tout les clients du serveur ainsi que leur informations:
		for(it = src.getClients().begin(); it != src.getClients().end(); it++)
		{
			o << "		- socket client: " << it->getSocket() << std::endl;
			o << "		- addresse client: " << it->getAddr().sin_port << std::endl;
			o << "		- name client: " << it->getName() << std::endl;
			o << "		- nickname client: " << it->getNickname() << std::endl;
			o << "		- password: " << it->getConnect() << std::endl;
			o << "	  	  ------------" << std::endl << std::endl;
		}

		// Affiche tout les client de tous les channels:
		std::vector<Channel>::const_iterator itChannel;
		for(itChannel = src.getChannels().begin() ; itChannel != src.getChannels().end(); itChannel++)
		{
			std::cout << "- Channel: " << itChannel->getName() << std::endl;
			std::cout << "- password: " << itChannel->getPasswd() << std::endl;
			std::cout << "- pwd: " << itChannel->getPwd() << std::endl;
			std::cout << "- socket owner: " << itChannel->getOwner() << std::endl;
			std::cout << "- topic name: " << itChannel->getTopicName() << std::endl;
			std::cout << "- topic restricted privilege: " << itChannel->getTopicPrivilege() << std::endl;
			std::cout << "- invation: " << itChannel->getInvitation() << std::endl;
			std::cout << "- max user:" << itChannel->getMaxUser() << std::endl;
			for( unsigned i = 0; i < itChannel->getIrcOps().size(); i++ )
				o << "- socket ircOps: " << itChannel->getIrcOps()[i] << std::endl;
			for( unsigned i = 0; i < itChannel->getUser().size(); i++ )
				o << "- socket user: " << itChannel->getUser()[i] << std::endl;
			for( unsigned i = 0; i < itChannel->getUserInvite().size(); i++ )
				o << "- user invitated: " << itChannel->getUserInvite()[i] << std::endl;
			o << "------------" << std::endl << std::endl;
		}
		std::cout << "-------------------------------------------"<< NC << std::endl;
		return( o );
	}

	void	Server::addInviteUser( int guestSocket, std::string channelName ){
		std::cout << "dasn Server :" << guestSocket << std::endl;	
		for (size_t i = 0; i < this->_channels.size(); i++){
		if (channelName == this->_channels[i].getName()){
			this->_channels[i].setUserInvite( guestSocket, PUSH );
			return ;
		}
	}
}

void	Server::changeTopic( std::string topic, std::string chanName, int idClient ){
	unsigned int j = 0; 
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (chanName == this->_channels[i].getName()){
			if (this->_channels[i].getTopicPrivilege() == true){
				while ( j < this->_channels[i].getIrcOps().size() ){
					if (  this->_channels[i].getIrcOps()[j] == idClient)
						break ;
					j++;
				}
				if ( j == this->_channels[i].getIrcOps().size() ){
						send(this->getSocket(),ERR_CHANOPRIVSNEEDED(this->_clients[idClient].getNickname(), this->_channels[i].getName()).c_str(), 
								ERR_CHANOPRIVSNEEDED(this->_clients[idClient].getNickname(), this->_channels[i].getName()).size(), 0);
						return;
				}
			}
			this->_channels[i].setTopicName( topic );
			this->_channels[i].setTopic( true );
		}
	}
}

std::string	Server::bufTmp( std::string buf, int flag, int index ){
	this->_clients[index].setBufTmp( buf, flag );
	return this->_clients[index].getBufTmp() ;
} 

void	Server::part( int socketClient, std::string chanName, std::string nick, std::string message){
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (chanName == this->_channels[i].getName()){
			if ( socketClient == this->_channels[i].getOwner() ){
				std::cout << " test " << std::endl;
				this->_channels.erase( this->_channels.begin() + i );
				return;
			}
			std::cout << message << std::endl;
			this->_channels[i].removeClientChannel( socketClient );
			for (unsigned int j = 0; j < this->_channels[i].getUser().size(); j++){
				if ( message.empty() ){
					std::cout << " test " << std::endl;
					send( this->_channels[i].getUser()[j] , RPL_PART(chanName, nick).c_str(), RPL_PART(chanName, nick).size(), 0);

				}
				else{
					send( this->_channels[i].getUser()[j] , RPL_PARTMSG(chanName, nick, message).c_str(), RPL_PARTMSG(chanName, nick, message).size(), 0);
					std::cout << " test kfd " << std::endl;
				}
			}
		}
	}
}
