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
		int			const & getSocket( void ) const;
		sockaddr_in const & getAddr( void ) const;
		std::string const & getName( void ) const;
		std::string const & getNickname( void ) const;
		bool		const & getConnect( void ) const;

		void				setSocket( int socket );
		void				setName( std::string name );
		void 				setNick( std::string nick, std::vector<Client> *clients );
		void 				setAddr( sockaddr_in addr );

		// -------------------- Functions ---------------------- //
		void				quit(); 
		void				join(Server *server, std::string join); 
		void				enterPwd(Server *server, std::string password); 
		void				privateMessage(std::vector<Client> *clients, Server *server, std::string info, int fdClient);
		int					checkRight( void );

		// --------------------------------------------------------- //
	private:
		int 		_socket;
		sockaddr_in	_address;
		std::string	_name;
		std::string	_nickname;
		bool		_connected;
		bool		_checkRight;
		bool		_inCanal;
};


# endif
