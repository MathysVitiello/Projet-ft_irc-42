#include "Server.hpp"
/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Server::Server( unsigned int const & port, std::string const & password  ): _port(port),
																		 	_password(password)
{
	this->_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_socket == -1 )
		throw std::runtime_error( "Don't opening socket." );

	this->_address.sin_addr.s_addr = INADDR_ANY;// toutes les sources acceptees.
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

	if ( this->_channels.empty() )
		this->_channels.erase( this->_channels.begin(), this->_channels.end() );
	this->_channels.clear();

	if ( this->_clients.empty() )
		this->_clients.erase( this->_clients.begin(), this->_clients.end() );
	this->_clients.clear();
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

void	Server::setClients( char *buf, int index ){
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
// 
// void	eraseOwnerChannel( int socket )
// {
// 
	// std::vector<Channel>::iterator it;
	// for(it = this->_channels.begin(); it != this->_channels.end(); it++)
	// {
			// 
	// }
// 
// }

void	Server::removeClient( int const & index )
{
	int socketClient = this->_clients[index].getSocket();
	std::string name = this->_clients[index].getName();

	// Supprime le client des differents channels:
	// I il est Owner ferme le channel
	std::vector<Channel>::iterator it;
	if ( this->_channels.empty() )
	{
		for(it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			for(size_t i = 0; i < it->getUser().size(); i++)
			{
				if( socketClient == it->getUser()[i] )
				{
					std::cout << "supresion du client [" << name << "] dans le tableau de channel " << it->getName() << std::endl;
					it->removeClientChannel( socketClient );
					break;
				}
			}
		}
	}
	std::cout << "--------" << std::endl;

	// Ferme le socket du client et l'efface du tableau de client:
	close( socketClient );
	this->_clients.erase( this->_clients.begin() + index );
}

void	Server::command(int fdClient){
	std::string	cmd[] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN", "KICK", "INVITE", "TOPIC", "MODE"};
	int i;

	for (i = 0; i < 10; i++){
		if(this->_clients[fdClient].getCmdBuf().empty()){
			i = -1; 
			break;
		}
		if(!this->_clients[fdClient].getCmdBuf()[0].find(cmd[i]))
			break;
	}
	switch (i) {
	case PASS:
		std::cout << "PASS  dans switch case " << std::endl;
		this->_clients[fdClient].enterPwd(this);
		break;
	case NICK:
		std::cout << "NICK  dans switch case " << std::endl;
		this->_clients[fdClient].setNick(this);
		break;
	case USER:
		std::cout << "USER   dans switch case " << std::endl;
		this->_clients[fdClient].setName();
		this->_clients[fdClient].removeCmdBuf();
		break;
	case PRIVMSG:
        std::cout << "PRIVMSG   dans switch case " << std::endl;
		// if (this->_clients[fdClient].checkRight() == true) //!
		this->_clients[fdClient].privateMessage(&this->_clients, this, fdClient);
		this->_clients[fdClient].removeCmdBuf();
		break;
	case JOIN:
        std::cout << "JOIN  dans switch case  " << std::endl;
        // if (this->_clients[fdClient].checkRight() == true)
		this->_clients[fdClient].join(this);
		this->_clients[fdClient].removeCmdBuf(); //! a changer a la fin, question nuil, est ce que a aprt pass nick user on peut tout faure sur une ligne

        break;
	case KICK:
		std::cout << "KICK a faire   dans switch case " << std::endl;
		break;
	case INVITE:
		// this->_clients[fdClient].invitation( this, cmdSend.substr(6) );
		std::cout << "INVITE a faire  dans switch case  " << std::endl;
		break;
	case TOPIC:
		// this->.topic( this->_clients[fdClient], cmdSend );
		std::cout << "TOPIC a faire   dans switch case " << std::endl;
		break;
	case MODE:
		std::cout << "MODE a faire  dans switch case " << std::endl;
		break;
	default:
		send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		this->_clients[fdClient].removeCmdBuf();
		break;
	}
}

// Verifie si le channel exsite:
bool Server::checkChannel( std::string name ){
	for(unsigned int i = 0; i  < this->_channels.size(); i++ )
	{
		if( this->getChannels()[i].getName() == name )
		{
			std::cout << "le canal [" << name << "] existe deja" << std::endl;
			return( true);
		}
	}
	return( false );
}

// Verifie si le client est dans le channel:
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

// si le canal est plein:
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

				// si le client n est pas dans le canal:
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
// 
// void	Server::createChannel( int clientSocket, std::string name, std::string passwd )
// {
	// if( this->checkChannel( name ) == true )
	// {
		// for(unsigned int i = 0; i  < this->_channels.size(); i++ )
		// {
			// if( this->getChannels()[i].getName() == name )
			// {
				// std::vector<int>::const_iterator ite = this->getChannels()[i].getUser().begin();
				// for (; ite != this->getChannels()[i].getUser().end(); ite++)
				// {
					// if( *ite == clientSocket )
					// {
						// char buff[4096] = "client deja dans le canal\r\n";
						// send(clientSocket, buff, strlen(buff), 0);
						// std::cout << "--------" << std::endl;
						// return;
					// }
				// }
				// std::vector<Client>::iterator it;
				// for( it = this->_clients.begin(); it != this->_clients.end(); it++)
				// {
					// if( it->getSocket() == clientSocket )
						// break;
				// }
				// si le client n est pas dans le canal:
				// this->_channels[i].addClientChannel( clientSocket );
				// char bufff[4096] = "client ajoute dans le canal\r\n";
				// send(clientSocket, bufff, strlen(bufff), 0);
// 
			// }
		// }
	// }
	// else	
	// {
		// std::cout << "Ajout du channel [" << name << "]" << std::endl;
		// Channel channel( clientSocket, name, passwd );
		// this->_channels.push_back( channel );
	// }
// 
// }
// 
void    Server::sendMessageChanel( int fdClient, std::string cmdSend)
{
    //pas oublier privmsg dans le cnaal avec kick et tout
    int i = 0;
    int senderFd = 0;

    std::vector<Client>::iterator it;
    for( it = this->_clients.begin(); it != this->_clients.end(); it++ )
    {
        if( it->getSocket() == fdClient )
            senderFd = i;
        i++;
    }
    for( size_t i = 0; i < this->getClients().size(); i++ )
    {
        if (fdClient != this->getClients()[i].getSocket())
        {
            //RPL_TOPIC(nick, channel, topic)
            send(this->getClients()[i].getSocket(), RPL_TOPIC(this->getClients()[senderFd].getNickname(), this->getChannels()[i].getName(), cmdSend).c_str(),
                RPL_TOPIC(this->getClients()[fdClient].getNickname(), this->getChannels()[i].getName(), cmdSend).size(), 0);
            send(this->getClients()[i].getSocket(), "\n", 1, 0);
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
		std::cout << "- max user:" << itChannel->getMaxUser() << std::endl;
		for( unsigned i = 0; i < itChannel->getIrcOps().size(); i++ )
			o << "- socket ircOps: " << itChannel->getIrcOps()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUser().size(); i++ )
			o << "- socket user: " << itChannel->getUser()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUserInvite().size(); i++ )
			o << "- user invitated: " << itChannel->getUser()[i] << std::endl;
	}
	std::cout << "-------------------------------------------"<< NC << std::endl;
	return( o );
}


