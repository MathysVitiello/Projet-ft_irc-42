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

		// -------------------- Functions ---------------------- //
		void	removeClientChannel( int index );

	// --------------------------------------------------------- //
	 private:
		std::string				_name;
		std::string				_password;
		std::vector<Client>		_user;	//User without privilege
											
		// unsigned int				_owner;  //Admin
		// std::vector<Client>			_ircOps; //Operator
		// 
		// std::string					_topicName;
		// std::vector<int>			_userInvitation;
		// std::string					_passChannel;
		// int							_maxClient;
	
				/* MODE  Active/Unactive */
		// bool						_pwd;
		// bool						_invitation;
		// bool						_topic;
		// bool						_privilege;
		// bool						_limitUser;

};

#endif
