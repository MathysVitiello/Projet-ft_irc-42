#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../library.hpp"
#include <string>

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
		int			const & getSocket( void ) const;
		sockaddr_in const & getAddr( void ) const;
		std::string const & getName( void ) const;
		std::string const & getNickname( void ) const;
		bool		const & getConnect( void ) const;
		std::vector<std::string>	const & getCmdBuf(void) const;
		std::string					getBufTmp( void );

		void				setSocket( int socket );
		void				setName( void );
		void 				setNick( Server *server );
		void 				setAddr( sockaddr_in addr );
		void				splitCmd( std::string cmdSend );
		void				setBufTmp( std::string buf, int flag );

		// -------------------- Functions ---------------------- //
		void				quit(); 
		void				join( Server *server );
		//TODO en construction kick
		void				kick( Server *server );
		void				enterPwd(Server *server); 
		void    			privateMessage( std::vector<Client> *clients, Server *server, int fdClient);
		int					checkRight( void );
		void				invitation( Server *server );
	
		void 				mode( Server *server );
		void				removeCmdBuf( void );
		void				parsHexchat( void );
		void				topic( Server *server );

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
		std::string				bufTmp;
};


# endif
