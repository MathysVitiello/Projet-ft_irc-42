#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../library.hpp"

class Channel{
	// --------------------------------------------------------- //
	 public:
		// Channel( unsigned int const & socket, unsigned in const & clientSocket );
		~Channel();
	 private:
		std::string					channelName;
		int							idChannel;
		unsigned int				owner;  //Admin
		std::vector<Client *>		ircOps; //Operator
		std::vector<Client *>		user;	//User without privilege
		
		std::string					topicName;
		std::vector<int>			userInvitation;
		std::string					passChannel;
		int							maxClient;
	
				/* MODE  Active/Unactive */
		bool						pwd;
		bool						invitation;
		bool						topic;
		bool						privilege;
		bool						limitUser;

};

#endif
