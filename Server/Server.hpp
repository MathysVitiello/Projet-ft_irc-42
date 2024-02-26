#ifndef SERVER_HPP
# define SERVER_HPP

# include "../library.hpp"

enum cmdUser {
	CAP,
	PASS,
	NICK,
	USER,
	PRIVMSG,
	JOIN,
	KICK,
	INVITE,
	TOPIC,
	MODE,
};

class Channel;
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
		std::vector<Channel>	const & getChannels( void  ) const;
		bool							getInCanal( Server * server, int fdClient );

		void							setClients( std::string buf, int index );


		// -------------------- Functions ---------------------- //
		void		addClient( int const & id, sockaddr_in from );
		void		command( int fdClient );
		void		removeClient( int const & index );
		void		createChannel( int clientSocket, std::string name, std::string passwd );
		bool		userInChannel( int i, int clientSocket );
		void		channelFull( int clientSocket );
		bool		checkChannel( std::string name );
		void   		sendMessageChanel( std::string nickOrChannel, int clientPlace, std::string cmdSend, int socket);
		void		eraseOwnerChannel( int socket );
		void		addInviteUser( int guestSocket, std::string channelName );
		void		changeTopic( std::string topic, std::string chanName );
		std::string	bufTmp( std::string buf, int flag, int index );
		void	channelInvit( Client *user, int i );

		// --------------------------------------------------------- //
	 private:
		Server( void );

		unsigned int 	const &		_port;	
		std::string		const &		_password;
		int							_socket;
		sockaddr_in					_address;
		std::vector<Client>			_clients;
		std::vector<Channel>		_channels;
};

// --------------------------------------------------------- //
// Fonctions non-membres:
void checkArgs( int argc, char **argv );

std::ostream & operator<<( std::ostream & o, Server const & src );

#endif
