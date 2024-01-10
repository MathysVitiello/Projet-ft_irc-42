#ifndef SERVER_HPP
# define SERVER_HPP

# include "../library.hpp"

enum cmdUser {
	PASS,
	NICK,
	USER,
	PRIVMSG,
	QUIT,
 };

class Client;
class Server
{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Server( unsigned int const & port, std::string const & password );
		~Server( void );

		// ----------------- Operator overload ----------------- //
		Client *operator[]( unsigned int index );

		// ------------------- Accessors ----------------------- //
		std::string				const & getPassword( void ) const;
		unsigned int			const & getPort( void ) const;
		int						const & getSocket( void ) const;
		sockaddr_in				const & getAddr( void ) const;
		std::vector<Client>		const & getClients( void  ) const;
		// std::vector<Channel>	const & getChannels( void  ) const;

		// -------------------- Functions ---------------------- //
		void	addClient( int const & id, sockaddr_in from );
		void	command(std::string cmdSend, int fdClient);
		// à faire
		// void	removeClient( int const & id, sockaddr_in from  );

	// --------------------------------------------------------- //
	 private:
		Server( void );

		unsigned int 	const & _port;	
		std::string		const &	_password;
		int						_socket;
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
