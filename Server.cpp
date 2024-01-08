#include "library.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Server::Server( unsigned int const & port, std::string const & password  ): _port(port),
																		 	_password(password)
{
	// -famille: AF_INET pour socket IPv4.
	// -type: SOCK_STREAM pour TCP.
	// -protocol: IPPROTO_TCP pour socket TCP.
	this->_fd= socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->_fd == -1 )
		throw std::runtime_error( "Invalid socket." );

	this->_address.sin_addr.s_addr = INADDR_ANY;// toutes les sources acceptees.
	this->_address.sin_port = htons( port );// traduit le port en reseau.
	this->_address.sin_family = AF_INET;// socket TCP IPv4.

	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Default constructor called" << std::endl;
	std::cout << "Server port: " << this->_port << std::endl;
	std::cout << "Server reseau: " << this->_address.sin_port<< std::endl;
	std::cout << "Server password: " << this->_password << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	return;
}

Server::~Server( void )
{
	std::cout << "Destructor called" << std::endl;
	this->_clients.clear();
	return;
}

/* ************************************************************************** */
// OPERATOR OVERLOAD:
Client const & Server::operator[](unsigned int index) const
{
	if ( index >= this->_clients.size() )
		 throw std::runtime_error( "Index is invalid" );
	return( this->_clients[index] );
}

/* ************************************************************************** */
// ACCESSORS:
unsigned int const & Server::getPort() const
{
	return( this->_port );
}

int	const & Server::getFd() const
{
	return( this->_fd );
}

std::string const & Server::getPassword() const
{
	return( this->_password );
}

std::vector<Client> const & Server::getClients( void ) const
{
	return( this->_clients );
}

sockaddr_in	const & Server::getAddr() const
{
	return( this->_address );
}

/* ************************************************************************** */
// FUNCTIONS:
// Rajoute un client dans le tableau de clients du serveur:
void	Server::addClient( int const & id, sockaddr_in addr)
{
	// max client 1024-> Fd_SETSIZE:
	Client *client = new Client(id, addr);
	this->_clients.push_back( *client );
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
		o << "- id client: " << it->getId() << std::endl;
		o << "- addresse client: " << it->getAddr().sin_port << std::endl;
		o << "- name client: " << it->getName() << std::endl;
		o << "- nickname client: " << it->getNickName() << std::endl;
	o << "--------" << std::endl;
	}
	std::cout << "-------------------------------------------" << std::endl;
	return( o );
}
