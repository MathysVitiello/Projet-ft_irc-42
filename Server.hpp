#ifndef SERVER_HPP
# define SERVER_HPP

# include "library.hpp"

class Server
{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Server( unsigned int const & port, std::string const & password );
		Server( Server const & src );
		~Server( void );

		// ----------------- Operator overload ----------------- //
		Server const & operator= ( Server const & src );
		Client const & operator[]( unsigned int index ) const;

		// ------------------- Accessors ----------------------- //
		std::string				const & getPassword( void ) const;
		unsigned int			const & getPort( void ) const;
		int						const & getFd( void ) const;
		sockaddr_in				const & getAddr( void ) const;
		// std::vector<Channel>	const & getChannels( void  ) const;
		std::vector<Client>		const & getClients( void  ) const;

		// -------------------- Functions ---------------------- //
		void	addClient( int const & id, sockaddr_in from );

	// --------------------------------------------------------- //
	 private:
		Server( void );

		unsigned int const & 	_port;	
		std::string	const &		_password;
		int						_fd;
		sockaddr_in				_address;
		// std::vector<Channel>	_channels;
		std::vector<Client>		_clients;
};

std::ostream & operator<<( std::ostream & o, Server const & src );
#endif
