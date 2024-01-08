#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "library.hpp"
#include <netinet/in.h>

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

		void				setName( std::string name );
		void 				setNick( std::string nick );
		void 				setAddr( sockaddr_in addr );

		// -------------------- Functions ---------------------- //

	// --------------------------------------------------------- //
	private:
		int 		_id;
		sockaddr_in	_address;
		std::string	_name;
		std::string	_nickName;
};

# endif
