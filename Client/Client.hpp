#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../library.hpp"

class Server;
class Client
{
	// --------------------------------------------------------- //
	public:
		// ------------- Constructor / Destructor -------------- //
		Client( int const & id, sockaddr_in from );
		~Client( void );

		// ----------------- Operator overload ----------------- //

		// ------------------- Accessors ----------------------- //
		int							const & getSocket( void ) const;
		sockaddr_in 				const & getAddr( void ) const;
		std::string 				const & getName( void ) const;
		std::string 				const & getNickname( void ) const;
		bool						const & getConnect( void ) const;
		bool						const & getConnectServer( void ) const;
		std::vector<std::string>	const & getCmdBuf(void) const;
		std::string					getBufTmp( void );

		void	setSocket( int socket );
		void	setName( std::vector<Client> *clients, Server *server, int fdClient );
		void 	setNick( Server *server );
		void 	setAddr( sockaddr_in addr );
		void	splitCmd( std::string cmdSend );
		void	setBufTmp( std::string buf, int flag );

		// -------------------- Functions ---------------------- //
		void    quit( Server *server, int fdClient, std::vector<Client> *clients);
		void	join( Server *server );
		void	kick( Server *server );
		void    capForHex( Server *server, int fdClient, std::vector<Client> *clients);

		void	enterPwd( Server *server ); 
		void	privateMessage( std::vector<Client> *clients, Server *server, int fdClient);
		void	checkRight( void );
		void	invitation( Server *server );
	
		void 	mode( Server *server );
		void 	splitBuf( void );

		void	removeCmdBuf( void );
		void	topic( Server *server );
		void	part( Server *server );

		// --------------------------------------------------------- //
	private:
		int 		_socket;
		sockaddr_in	_address;
		std::string	_name;
		std::string	_nickname;
		bool		_connected;
		bool		_checkRight;
		std::string					_cmdTmp;
		std::vector<std::string>	_splitBuf;  // 0: CMD 1:ARG 2:ARG ...
		std::string					bufTmp;
};


# endif
