#include "Server.hpp"

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
	// std::cout << "Destructor called" << std::endl;
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

std::vector<Client> const & Server::getClients( void ) const{
	return( this->_clients );
}

sockaddr_in	const & Server::getAddr() const{
	return( this->_address );
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
	int retour = this->_clients[index].getSocket();
	std::string name = this->_clients[index].getName();

	std::cout << "- name client remove : " << name  << std::endl;
		std::vector<Channel>::iterator it;
	if ( this->_channels.empty() )
	{
		for(it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			std::string name = it->getUser()[index].getName();
			for(size_t i = 0; i < it->getUser().size(); i++)
			{
				if( name == it->getUser()[i].getName() )
				{
					std::cout << "supresion dans le tableau de channel " << name << std::endl;
					it->removeClientChannel(i);
					break;

				}
				else
				{
					std::cout << "- name client: " << it->getUser()[i].getName() << std::endl;
					std::cout << "- socket client: " << it->getUser()[i].getSocket() << std::endl;
					std::cout << "- addresse client: " << it->getUser()[i].getAddr().sin_port << std::endl;
					std::cout << "- nickname client: " << it->getUser()[i].getNickname() << std::endl;
					std::cout << "- password: " << it->getUser()[i].getConnect() << std::endl;
					std::cout << "--------" << std::endl;
				}
			}
		}
	}
	close( retour );
	this->_clients.erase( this->_clients.begin() + index );

}

/* ************************************************************************** */
// NO-MENBER'S FUNCTIONS:
// Verifie la validitee des arguments:
void	checkArgs(int argc, char **argv)
{
	if (argc != 3)
			throw std::runtime_error( "Not the right number of arguments" ) ;

	std::string    nb = argv[1];
	for (unsigned int i = 0; i < nb.length(); i++)
		if (!isdigit (nb[i]))
			throw std::runtime_error( "the First argument is the port, pls write a number!" );
	if (std::string(argv[2]).length() == 0)
        throw std::runtime_error("write password, pls.");
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
		if (this->_clients[fdClient].checkRight() == true)
			this->_clients[fdClient].privateMessage(&this->_clients, cmdSend.substr(7));
		break;
	case JOIN:
        std::cout << "JOIN " << std::endl;
        if (this->_clients[fdClient].checkRight() == true)
            this->_clients[fdClient].join(this, cmdSend.substr(4));
        break;
	default:
		send(this->_clients[fdClient].getSocket(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).c_str(), ERR_UNKNOWNCOMMAND(this->_clients[fdClient].getNickname()).size(), 0);
		break;
	}
}

void	Server::createChannel( Client * client, std::string name, std::string passwd )
{
	// int ret = 0;
	Channel channel( client, name, passwd );

	this->_channels.push_back( channel );
// 
	// for(std::vector<Channel>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	// {
		// 
		// if( it->getName() == name )// verifier si le client est dans le cannal
		// {
			// std::cout << "le canal exite deja" << std::endl;
			// ret = 1;
			// break;
		// }
	// }
	// if( ret == 0 )
	// {
		// std::cout << "le canal nexiste pas" << std::endl;
		// Channel channel( client, name, passwd );
// 
		// this->_channels.push_back( channel );
// 
// 
	// }


	for(std::vector<Channel>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		std::cout << "channel [" << it->getName() << "]" << std::endl;
		for(size_t i = 0; i < it->getUser().size(); i++)
		{
			std::cout << "- name client: " << it->getUser()[i].getName() << std::endl;
			std::cout << "- socket client: " << it->getUser()[i].getSocket() << std::endl;
			std::cout << "- addresse client: " << it->getUser()[i].getAddr().sin_port << std::endl;
			std::cout << "- nickname client: " << it->getUser()[i].getNickname() << std::endl;
			std::cout << "- password: " << it->getUser()[i].getConnect() << std::endl;
			std::cout << "--------" << std::endl;

		}
	}
	char buf[4096] = "client ok\r\n";
	send(client->getSocket(), buf, strlen(buf), 0);
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


