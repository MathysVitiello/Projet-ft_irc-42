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
		bool					const & getPwd ( void ) const;
		bool					const & getTopic ( void ) const;
		std::string				const & getTopicName( void ) const;
		bool					const & getTopicPrivilege( void ) const;
		bool					const & getInvitation ( void ) const;
		std::vector<int>		const & getUserInvite ( void ) const;
		long int				const & getMaxUser( void ) const;
						
		void			setTopicName( std::string topic );
		void			setTopic( bool topic );
		void			setInvitation( bool invitation );
		void			setTopicPrivilege( bool privilege );
		void			setUserInvite ( int socketInvite, int flag );
		void			setPassword( std::string password );
		void			setPwd( bool pwd );
		void			setMaxUser( long int limit );
		
		// -------------------- Functions ---------------------- //
		bool	removeIrcOps( int clientSocket );
		void	removeClientChannel( int index );
		bool	addClientChannel( int clientSocket );
		bool	addIrcOps( int clientSocket );


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
		long int			_maxUser;
	
				/* MODE  Active/Unactive */
		bool						_pwd;
		bool						_topic;
		bool						_privilege;
};

#endif
