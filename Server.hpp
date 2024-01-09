#ifndef SERVER_HPP
# define SERVER_HPP

# include "library.hpp"

class Server
{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Server( unsigned int const & port, std::string const & password );
		~Server( void );

		// ------------------- Accessors ----------------------- //
		std::string				const & getPassword( void ) const;
		unsigned int			const & getPort( void ) const;
		int						const & getFd( void ) const;
		sockaddr_in				const & getAddr( void ) const;
		std::vector<Client>		const & getClients( void  ) const;
		// void setClients( std::vector<Client> _clients, int index );
		// std::vector<Channel>	const & getChannels( void  ) const;

		// -------------------- Functions ---------------------- //
		void	addClient( int const & fd, sockaddr_in address );

	// --------------------------------------------------------- //
	 private:
		Server( void );

		unsigned int 	const & _port;	
		std::string		const &	_password;
		int						_fd;
		sockaddr_in				_address;
		// fd_set _master;
		std::vector<Client>		_clients;
		// std::vector<Channel>	_channels;
};

// --------------------------------------------------------- //
// Fonctions non-membres:
void checkArgs( int argc, char **argv );

std::ostream & operator<<( std::ostream & o, Server const & src );

#endif
