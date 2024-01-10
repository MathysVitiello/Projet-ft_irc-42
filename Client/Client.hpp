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
		int			const & getId( void ) const;
		sockaddr_in const & getAddr( void ) const;
		std::string const & getName( void ) const;
		std::string const & getNickName( void ) const;
		bool		const & getConnect( void ) const;

		void				setName( std::string name );
		void 				setNick( std::string nick, std::vector<Client> *clients );
		void 				setAddr( sockaddr_in addr );

		// -------------------- Functions ---------------------- //
		void				quit(); //appelle 1 fonction Server qui detruit le Client dans vector
		void				join(); //creer 1 canal ou en join 1
		bool				enterPwd(Server *server, std::string password); //verif "PASS" + bon password; si bon change _connected


		// --------------------------------------------------------- //
	private:
		int 		_id;
		sockaddr_in	_address;
		std::string	_name;
		std::string	_nickName;
		bool		_connected;
};

# endif
