#ifndef SERVER_HPP
# define SERVER_HPP

# include "../library.hpp"
#include <string>

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
	PART,
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
		void		createChannel( Client client, std::string name, std::string passwd );
		bool		userInChannel( int i, int clientSocket );
		void		channelFull( int clientSocket );
		bool		checkChannel( std::string name );
		void   		sendMessageChanel( std::string nickOrChannel, int clientPlace, std::string cmdSend, int socket);
		void		eraseOwnerChannel( int socket );
		void		addInviteUser( int guestSocket, std::string channelName );
		bool		changeTopic( std::string topic, std::string chanName, int idClient, std::string nick );
		std::string	bufTmp( std::string buf, int flag, int index );
		void		part( int socketClient, std::string chan, std::string nick, std::string message);
		void		kickUser( int socketToKick, std::string channelName, std::string message );
		void		allClient( Channel *chan, Client client );

		void		modeInvit( Client *user, int i );
		void		modeTopic( Client *user, int nChannel );
		void		modePrivilege( Client *user, int i );
		void		modePwd( Client *user, int i );
		void		modeLimitUser( Client *user, int i );
		void		nickChange( std::string oldNick, std::string newNick, int socket );


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
