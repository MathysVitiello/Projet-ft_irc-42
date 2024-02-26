#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../library.hpp"

#define PUSH  0
#define DEL  1 

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
		bool					const & getTopic ( void ) const;
		std::string				const & getTopicName( void ) const;
		bool					const & getInvitation ( void ) const;
		std::vector<int>		const & getUserInvite ( void ) const;
		int						const & getMaxUser( void ) const;
						
		// void				setUSER( int index );
		void			setTopicName( std::string topic );
		void			setTopic( bool topic );
		void			setInvitation( bool invitation );
		void			setUserInvite ( int socketInvite, int flag );
		
		// -------------------- Functions ---------------------- //
		void	removeClientChannel( int index );
		bool	addClientChannel( int clientSocket );


	// --------------------------------------------------------- //
	 private:
		std::string			_name;
		std::string			_password;
		int 				_owner;  //Admin
		std::vector<int>	_ircOps; //Operator
		std::vector<int>	_user;								
		std::string			_topicName;
		bool				_invitation;
		std::vector<int>	_userInvitation;
		int					_maxUser;
	
				/* MODE  Active/Unactive */
		// bool						_pwd;
		bool						_topic;
		// bool						_privilege;
};

#endif
