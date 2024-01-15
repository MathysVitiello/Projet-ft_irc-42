#include "Server.hpp"
#include <algorithm> 
/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Server::Server( unsigned int const & port, std::string const & password  ): _port(port),
																		 	_password(password)
{
	this->_socket= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_socket == -1 )
		throw std::runtime_error( "Invalid socket." );

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

	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Server port: " << this->_port << std::endl;
	std::cout << "Server reseau: " << this->_address.sin_port<< std::endl;
	std::cout << "Server password: " << this->_password << std::endl;
	std::cout << "Server socket: " << this->_socket << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
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
	std::cout << "Server destroy" << std::endl;
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

/* ************************************************************************** */
// FUNCTIONS:
// Rajoute un client dans le tableau de clients du serveur:
void	Server::addClient( int const & id, sockaddr_in addr)
{
	// max client 1024-> Fd_SETSIZE:
	Client client(id, addr);
	this->_clients.push_back( client );
}

void	Server::removeClient( int const & index )
{
	int socketClient = this->_clients[index].getSocket();
	std::string name = this->_clients[index].getName();

	// Supprime le client des differents channels:
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
	std::cout << "Client [" << name << "] est dans aucun channel" << std::endl;
	std::cout << "--------" << std::endl;

	// Ferme le socket du client et l'efface du tableau de client:
	close( socketClient );
	this->_clients.erase( this->_clients.begin() + index );
}

void	Server::command(std::string cmdSend, int fdClient){
	std::string	cmd[] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN"};
	int i;

	for (i = 0; i < 5; i++){
		if(!cmdSend.find(cmd[i]))
			break;
	}
	switch (i) {
	case PASS:
		std::cout << "PASS " << std::endl;
		this->_clients[fdClient].enterPwd(this, cmdSend.substr(4));
		break;
	case NICK:
		std::cout << "NICK " << std::endl;
		this->_clients[fdClient].setNick(cmdSend.substr(4), &this->_clients);
		break;
	case USER:
		std::cout << "USER " << std::endl;
		this->_clients[fdClient].setName(cmdSend.substr(4));
		break;
	case PRIVMSG:
        std::cout << "PRIVMSG " << std::endl;
		if (this->_clients[fdClient].checkRight() == true)
			this->_clients[fdClient].privateMessage(&this->_clients, this ,cmdSend.substr(7), fdClient);
		break;
	case JOIN:
        std::cout << "JOIN " << std::endl;
        if (this->_clients[fdClient].checkRight() == true)
            this->_clients[fdClient].join(this, cmdSend.substr(4));
        break;
	default:
		if (getInCanal(this, fdClient) == true)
			commandChannel(cmdSend, fdClient);
		else
			send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		break;
	}
}

void	Server::commandChannel( std::string cmdSend, int fdClient)
{
	std::string	cmd[] = {"KICK", "INVITE", "TOPIC", "MODE"};
	int i;

	for (i = 0; i < 4; i++){
		if(!cmdSend.find(cmd[i]))
			break;
	}
	switch (i) {
	case KICK:
		std::cout << "KICK a faire" << std::endl;
		break;
	case INVITE:
		std::cout << "INVITE a faire" << std::endl;
		break;
	case TOPIC:
		std::cout << "TOPIC a faire" << std::endl;
		break;
	case MODE:
		std::cout << "MODE a faire" << std::endl;
		break;
	default:
		send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		break;
	}
	return ;
}

// Verifie si le channel exsite:
unsigned int Server::checkChannel( std::string name ){
	for(unsigned int i = 0; i < this->_channels.size(); i++ )
	{
		if( this->getChannels()[i].getName() == name )
		{
			std::cout << "le canal [" << name << "] existe deja" << std::endl;
			return( i );
		}
	}
	return( 0 );
}

void	Server::createChannel( int clientSocket, std::string name, std::string passwd )
{
	int ret = 0;
	unsigned int index = this->checkChannel( name );

	if( index != 0 )
	{
		ret = 1;
		std::vector<Client>::iterator it;
		int i = 0;
		for( it = this->_clients.begin(); it != this->_clients.end(); it++, i++ )
		{
			if( it->getSocket() == clientSocket )
			{
				char buff[4096] = "client deja dans le canal\r\n";
				send(clientSocket, buff, strlen(buff), 0);
				std::cout << "--------" << std::endl;
				return;
			}
		}
		// si le client n est pas dans le canal:
		this->_channels[index].addClientChannel( clientSocket );
		char bufff[4096] = "client ajoute dans le canal\r\n";
		send(clientSocket, bufff, strlen(bufff), 0);
	}
		
	if( ret == 0 )
	{
		std::cout << "Ajout du channel [" << name << "]" << std::endl;
		Channel channel( clientSocket, name, passwd );
		this->_channels.push_back( channel );
	}

	//Recherche le client par rapport a la socket:
	//et affiche tout les client du channel:
	index = this->checkChannel( name );
	std::vector<int>::const_iterator i = this->_channels[index].getUser().begin();
	std::cout << "Channel " << name << std::endl;
	for (; i != this->_channels[index].getUser().end(); i++)
		 std::cout << "- socket client: " << *i << std::endl;

}

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

	std::cout << "-------------------------------------------" << std::endl;
	o << "Server port: " << src.getPort() << std::endl;
	o << "Server reseau: " << src.getAddr().sin_port<< std::endl;
	o << "Server password: " << src.getPassword() << std::endl;
	o << "--------" << std::endl;

	for(it = src.getClients().begin(); it != src.getClients().end(); it++)
	{
		o << "| " << it->getSocket() << " | " << it->getAddr().sin_port << std::endl;
		o << "- socket client: " << it->getSocket() << std::endl;
		o << "- addresse client: " << it->getAddr().sin_port << std::endl;
		o << "- name client: " << it->getName() << std::endl;
		o << "- nickname client: " << it->getNickname() << std::endl;
		o << "- password: " << it->getConnect() << std::endl;
		o << "--------" << std::endl;
	}
	std::cout << "-------------------------------------------" << std::endl;
	return( o );
}

