#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "library.hpp"

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

		// -------------------- Functions ---------------------- //

	// --------------------------------------------------------- //
	private:
		int 		_id;
		sockaddr_in	_address;
		std::string	_name;
		std::string	_nickName;
};

# endif
