#include "Server.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Server::Server( unsigned int const & port, std::string const & password  ): _port(port),
																		 	_password(password)
{
	this->_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_socket == -1 )
		throw std::runtime_error( "Don't opening socket." );

	this->_address.sin_addr.s_addr = INADDR_ANY;// accepted sources
	this->_address.sin_port = htons( port );	// traduit le port en reseau.
	this->_address.sin_family = AF_INET;		// socket TCP IPv4.
										
	// configures the server and puts it on standby:
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
			if( server->_clients[fdClient].getSocket() == it->getUser()[i] )
				return true;
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
		if (this->_clients[fdClient].getConnectServer() == true)
			this->_clients[fdClient].join(this);
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
			return( true);
	}
	return( false );
}

// Verify if client is in the channel
bool	Server::userInChannel( int i, int clientSocket )
{
	std::string nick;
	std::string server = "irc";
	for(unsigned int i = 0; i  < this->_clients.size(); i++ )
	{
		if( clientSocket == this->_clients[i].getSocket() )
		{
			nick = this->_clients[i].getName();
			break;
		}
	}

	std::vector<int>::const_iterator ite = this->getChannels()[i].getUser().begin();
	for (; ite != this->getChannels()[i].getUser().end(); ite++)
	{
		if( *ite == clientSocket )
		{
			send(clientSocket, ERR_USERONCHANNEL(server, nick, this->getChannels()[i].getName()).c_str(),
					ERR_USERONCHANNEL(server, nick, this->getChannels()[i].getName()).size(), 0);
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
				if( static_cast<unsigned>(this->_channels[i].getMaxUser()) <= this->_channels[i].getUser().size() )
				{
					send(client.getSocket(), ERR_CHANNELISFULL(client.getNickname(), this->_channels[i].getName()).c_str(),
						ERR_CHANNELISFULL(client.getNickname(), this->_channels[i].getName()).size(), 0);
					return;
				}

				if( !this->userInChannel( i, clientSocket ) )
					return;

				// if client is not in channel
				if( this->_channels[i].getPwd() == true )
				{
					if( passwd != this->_channels[i].getPasswd() )
					{
						send(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), this->_channels[i].getName()).c_str(),
							ERR_BADCHANNELKEY(client.getNickname(), this->_channels[i].getName()).size(), 0);
						return;
					}
					send(client.getSocket(), RPL_CHANNELMODEIS2(client.getNickname(), this->_channels[i].getName(), "+k", passwd).c_str(), 
						RPL_CHANNELMODEIS2(client.getNickname(), this->_channels[i].getName(), "+k", passwd).size(), 0);
				}
				if( this->_channels[i].getInvitation() == true )
				{
					send(client.getSocket(), ERR_INVITEONLYCHAN(client.getName(), this->_channels[i].getName()).c_str(),
						ERR_INVITEONLYCHAN(client.getName(), this->_channels[i].getName()).size(), 0);
					return;
				}
				if( this->_channels[i].addClientChannel(clientSocket) == true )
				{
					this->allClient(&this->_channels[i], client);

					int nbChannel = 0;
					// check si le channel existe
					for( size_t i = 0; i < this->getChannels().size(); i++ )
						if (this->getChannels()[i].getName() == name)
							nbChannel = i;
					if (this->getChannels()[nbChannel].getUser().size() > 1)
					{
						for( size_t i = 0; i < this->_clients.size(); i++ )
							if (clientSocket == this->_clients[i].getSocket())
								socketNewUser = i;

						for( size_t i = 0; i < this->getChannels()[nbChannel].getUser().size(); i++ )
						{
							//send to all people from channel message that someone new joined
							if (clientSocket != this->getChannels()[nbChannel].getUser()[i])
								send(this->getChannels()[nbChannel].getUser()[i], RPL_CHAN(this->_clients[socketNewUser].getNickname(), 
									"JOIN", this->getChannels()[nbChannel].getName()).c_str(), RPL_CHAN(this->_clients[socketNewUser].getNickname(), 
									"JOIN", this->getChannels()[nbChannel].getName()).size(), 0);
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

void	Server::addInviteUser( int guestSocket, std::string channelName ){
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (channelName == this->_channels[i].getName())
		{
			this->_channels[i].setUserInvite( guestSocket, PUSH );
			return ;
		}
	}
}

bool	Server::changeTopic( std::string topic, std::string chanName, int idClient, std::string nick ){
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
					send( idClient,ERR_CHANOPRIVSNEEDED(nick, this->_channels[i].getName()).c_str(), 
						ERR_CHANOPRIVSNEEDED(nick, this->_channels[i].getName()).size(), 0);
					return false;
				}
			}
			this->_channels[i].setTopicName( topic );
			this->_channels[i].setTopic( true );
		}
	}
	return true;
}

std::string	Server::bufTmp( std::string buf, int flag, int index ){
	this->_clients[index].setBufTmp( buf, flag );
	return this->_clients[index].getBufTmp() ;
} 

void	Server::part( int socketClient, std::string chanName, std::string nick, std::string message){
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (chanName == this->_channels[i].getName()){
			if ( socketClient == this->_channels[i].getOwner() ){
				this->_channels.erase( this->_channels.begin() + i );
				return;
			}
			for (unsigned int j = 0; j < this->_channels[i].getUser().size(); j++)
				send( this->_channels[i].getUser()[j] , RPL_PART(nick, chanName, message).c_str(),
					RPL_PART(nick, chanName, message).size(), 0);
			this->_channels[i].removeClientChannel( socketClient );
		}
	}
}

// Displays all data included in the server:
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

	// Displays all server clients and their information:
	for(it = src.getClients().begin(); it != src.getClients().end(); it++)
	{
		o << "		- socket client: " << it->getSocket() << std::endl;
		o << "		- addresse client: " << it->getAddr().sin_port << std::endl;
		o << "		- name client: " << it->getName() << std::endl;
		o << "		- nickname client: " << it->getNickname() << std::endl;
		o << "		- password: " << it->getConnect() << std::endl;
		o << "	  	  ------------" << std::endl << std::endl;
	}

	// Displays all customers on all channels:
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
