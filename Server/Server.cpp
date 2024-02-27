#include "Server.hpp"
#include <string>
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
	std::cout << "SERVEUR EN ROUTE..." << std::endl;
	std::cout << " -Adresse( emplacement ) memoire: " << this << std::endl;
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

void	Server::command(int fdClient){

	std::string	cmd[] = {"CAP", "PASS", "NICK", "USER", "PRIVMSG", "JOIN", "KICK", "INVITE", "TOPIC", "MODE"};
	int i;

	for (i = 0; i < 11; i++){
		size_t j = this->_clients[fdClient].getCmdBuf()[0].find('\r');

		if(this->_clients[fdClient].getCmdBuf().empty()){
			i = -1; 
			break;
		}
		if(this->_clients[fdClient].getCmdBuf()[0] == cmd[i])
			break;
		if(j != std::string::npos)
			this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);	
	}

	switch (i) {
	case CAP:
		std::cout << "CAP jarrive " << std::endl;
		this->_clients[fdClient].capForHex(this, fdClient, &this->_clients);
		break;
	case PASS:
		std::cout << "PASS  dans switch case " << std::endl;
		this->_clients[fdClient].enterPwd(&this->_clients, this, fdClient);
		break;
	case NICK:
		std::cout << "NICK  dans switch case " << std::endl;
		this->_clients[fdClient].setNick(&this->_clients, this, fdClient);
		break;
	case USER:
		std::cout << "USER   dans switch case " << std::endl;
		this->_clients[fdClient].setName(&this->_clients, this, fdClient);
		this->_clients[fdClient].removeCmdBuf();
		break;
	case PRIVMSG:
        std::cout << "PRIVMSG   dans switch case " << std::endl;
		if (this->_clients[fdClient].checkRight() == true)
			this->_clients[fdClient].privateMessage(&this->_clients, this, fdClient);
		this->_clients[fdClient].removeCmdBuf();
		break;
	case JOIN:
        std::cout << "JOIN  dans switch case  " << std::endl;
        if (this->_clients[fdClient].checkRight() == true){
			std::cout << "ici" << std::endl;
			this->_clients[fdClient].join(this);
		}
		this->_clients[fdClient].removeCmdBuf();
        break;
	case KICK:
		//pour l'operateur uniquement
		std::cout << "KICK a faire   dans switch case " << std::endl;
		this->_clients[fdClient].kick(this);
		this->_clients[fdClient].removeCmdBuf();
		break;
	case INVITE:
		this->_clients[fdClient].invitation( this );
		std::cout << "INVITE a faire  dans switch case  " << std::endl;
		this->_clients[fdClient].removeCmdBuf();
		break;
	case TOPIC:
		this->_clients[fdClient].topic( this );
		std::cout << "TOPIC a faire   dans switch case " << std::endl;
		this->_clients[fdClient].removeCmdBuf();
		break;
	case MODE:
		std::cout << "MODE" << std::endl;
		this->_clients[fdClient].mode(this);
		this->_clients[fdClient].removeCmdBuf();
		break;
	default:
		send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		this->_clients[fdClient].removeCmdBuf();
		break;
	}
}

// Verify if channel exists
bool Server::checkChannel( std::string name ){
	for(unsigned int i = 0; i  < this->_channels.size(); i++ )
	{
		if( this->getChannels()[i].getName() == name )
		{
			//! stdError a faire, et english 
			std::cout << "le canal [" << name << "] existe deja" << std::endl;
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
			char buff[4096] = "client deja dans le canal\r\n";
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

void	Server::createChannel( int clientSocket, std::string name, std::string passwd )
{
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
					char bufff[4096] = "client ajoute dans le canal\r\n";
					send(clientSocket, bufff, strlen(bufff), 0);
				}
				else
					this->channelFull( clientSocket );
			}
		}
	}
	else	
	{
		std::cout << "Ajout du channel [" << name << "]" << std::endl;
		Channel channel( clientSocket, name, passwd );
		this->_channels.push_back( channel );
	}
}


void	Server::channelInvit( Client *user, int i )
{
	// invite mode
	if( user->getCmdBuf()[1] == "+i" )	
	{
		if( this->_channels[i].getInvitation() == false )
		{
			this->_channels[i].setInvitation( true );
			std::cout << "le canal passe en mode invit" << std::endl;
			return;
		}
		else
		{
			// le channel est deaj sur le mode invitation
			std::cout << "le canal est deja en mode invit" << std::endl;
		}
	}
	//enleve le mode invit
	else if( user->getCmdBuf()[1] == "-i" )	
	{
		if( this->_channels[i].getInvitation() == true )
		{
			this->_channels[i].setInvitation( false );
			std::cout << "le canal s enleve du mode invit" << std::endl;
		}
		else
		{
			std::cout << "le canal est deja enlever du mode invit" << std::endl;
			// le channel est deja PAS sur le mode invitation
		}
	}
}

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
	o << "--------" << std::endl;

	// Affiche tout les clients du serveur ainsi que leur informations:
	for(it = src.getClients().begin(); it != src.getClients().end(); it++)
	{
		o << "- socket client: " << it->getSocket() << std::endl;
		o << "- addresse client: " << it->getAddr().sin_port << std::endl;
		o << "- name client: " << it->getName() << std::endl;
		o << "- nickname client: " << it->getNickname() << std::endl;
		o << "- password: " << it->getConnect() << std::endl;
		o << "--------" << std::endl;
	}

	// Affiche tout les client de tous les channels:
	std::vector<Channel>::const_iterator itChannel;
	for(itChannel = src.getChannels().begin() ; itChannel != src.getChannels().end(); itChannel++)
	{
		std::cout << "- Channel: " << itChannel->getName() << std::endl;
		std::cout << "- password: " << itChannel->getPasswd() << std::endl;
		std::cout << "- socket owner: " << itChannel->getOwner() << std::endl;
		std::cout << "- topic name: " << itChannel->getOwner() << std::endl;
		std::cout << "- invation: " << itChannel->getInvitation() << std::endl;
		std::cout << "- max user:" << itChannel->getMaxUser() << std::endl;
		for( unsigned i = 0; i < itChannel->getIrcOps().size(); i++ )
			o << "- socket ircOps: " << itChannel->getIrcOps()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUser().size(); i++ )
			o << "- socket user: " << itChannel->getUser()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUserInvite().size(); i++ )
			o << "- user invitated: " << itChannel->getUserInvite()[i] << std::endl;
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

void	Server::changeTopic( std::string topic, std::string chanName ){
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (chanName == this->_channels[i].getName()){
			this->_channels[i].setTopicName( topic );
			this->_channels[i].setTopic( true );
		}
	}
}

std::string	Server::bufTmp( std::string buf, int flag, int index ){
	this->_clients[index].setBufTmp( buf, flag );
	return this->_clients[index].getBufTmp() ;
} 
