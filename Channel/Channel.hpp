#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../library.hpp"

class Client;
class Channel{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Channel( int userSocket, std::string name, std::string password );
		~Channel( void );

		int	operator[]( unsigned int index );
		// ------------------- Accessors ----------------------- //
		std::string				const & getName( void ) const;
		std::string				const & getPasswd( void ) const;
		int						const & getOwner( void ) const;
		std::vector<int>		const & getIrcOps ( void ) const;
		std::vector<int>		const & getUser ( void ) const;
		// void				setUSER( int index );

		// -------------------- Functions ---------------------- //
		void	removeClientChannel( int index );
		void	addClientChannel( int clientSocket );

	// --------------------------------------------------------- //
	 private:
		std::string			_name;
		std::string			_password;
		int 				_owner;  //Admin
		std::vector<int>	_ircOps; //Operator
		std::vector<int>	_user;								
		// std::string		_topicName;
		// std::vector<int>	_userInvitation;
		int					_maxUser;
	
				/* MODE  Active/Unactive */
		// bool						_pwd;
		// bool						_invitation;
		// bool						_topic;
		// bool						_privilege;
};

#endif
