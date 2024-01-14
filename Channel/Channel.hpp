#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../library.hpp"

class Client;
class Channel{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Channel( Client *user, std::string name, std::string password );
		~Channel( void );

		Client *operator[]( unsigned int index );
		// ------------------- Accessors ----------------------- //
		std::string				const & getName( void ) const;
		std::string				const & getPasswd( void ) const;
		std::vector<Client>		const & getUser ( void ) const;
		void				setUSER( int index );
		// -------------------- Functions ---------------------- //
		void	removeClientChannel( int index );

	// --------------------------------------------------------- //
	 private:
		std::string		_name;
		std::string		_password;
		// int			_owner;  //Admin
		std::vector<Client>	_user;	//User without privilege
		// std::vector<int>	_user;								
		// std::vector<int>	_ircOps; //Operator
		// std::string		_topicName;
		// std::vector<int>	_userInvitation;
		// int			_maxClient;
	
				/* MODE  Active/Unactive */
		// bool						_pwd;
		// bool						_invitation;
		// bool						_topic;
		// bool						_privilege;
};

#endif
