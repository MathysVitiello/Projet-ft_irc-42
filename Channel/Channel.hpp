#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../library.hpp"
#include <netinet/in.h>

class Channel{
	// --------------------------------------------------------- //
	 public:
		// ------------- Constructor / Destructor -------------- //
		Channel( std::string name );
		~Channel( void );

		// ------------------- Accessors ----------------------- //
		std::string		const & getName( void ) const;
		int				const & getSocket( void ) const;
		sockaddr_in		const & getAddr( void ) const;

		// -------------------- Functions ---------------------- //
		fd_set	createFdSet();

	// --------------------------------------------------------- //
	 private:
		std::string				_name;
		int						_socket;
		sockaddr_in				_address;
		fd_set					_master;

		// unsigned int				_owner;  //Admin
		// std::vector<Client>			_ircOps; //Operator
		// std::vector<Client>			_user;	//User without privilege
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
