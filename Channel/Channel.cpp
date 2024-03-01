#include "Channel.hpp"

/* ************************************************************************** */
// CONSTRUCTOR / DESTRUCTOR:
Channel::Channel( int userSocket, std::string name, std::string mdp) :
	_name( name ),
	_password( mdp ),
	_owner( userSocket ),
	_invitation( false ),
	_maxUser( FD_SETSIZE ),
	_topic( false ),
	_privilege( false ){
	std::cout << "Channel [" << this->_name << "] created Owner socket: " << userSocket << std::endl;

	this->_ircOps.push_back( userSocket );
	this->_user.push_back( userSocket );
}

Channel::~Channel( void ){
	std::cout << "channel ["<< this->_name << "] closed" << std::endl;

	this->_ircOps.clear();
	this->_user.clear();
	this->_userInvitation.clear();
}

/* ************************************************************************** */
// OPERATOR OVERLOAD:
int Channel::operator[](unsigned int index) {
	if ( index >= this->_user.size() )
		 throw std::runtime_error( "Index is invalid" );
	return this->_user[index];
}

/* ************************************************************************** */
// ACCESSORS:
std::string	const & Channel::getName( void ) const{
	return( this->_name);
}

std::string	const & Channel::getPasswd( void ) const{
	return( this->_password );
}

int const &	Channel::getOwner( void ) const{
	return( this->_owner );
}

std::vector<int>	const & Channel::getIrcOps( void ) const{
	return( this->_ircOps);
}

std::vector<int>	const & Channel::getUser( void ) const{
	return( this->_user);
}

std::string	const & Channel::getTopicName( void ) const{
	return	this->_topicName;
}

bool	const & Channel::getTopic( void ) const{
	return this->_topic;
}

bool	const & Channel::getTopicPrivilege( void ) const{
	return this->_privilege;
}

bool		const & Channel::getInvitation ( void ) const{
	return ( this->_invitation );
}

std::vector<int>		const & Channel::getUserInvite ( void ) const{
	return ( this->_userInvitation );
}

int	const & Channel::getMaxUser( void ) const{
	return( this->_maxUser );
}

void	Channel::setInvitation( bool invitation ){
	this->_invitation = invitation;
}

void	Channel::setTopicPrivilege( bool privilege ){
	this->_privilege = privilege;
}
void	Channel::setTopicName( std::string topic ){
	this->_topicName = topic;
}

void	Channel::setTopic( bool topic ){
	this->_topic = topic;
}

/* ************************************************************************** */
// FUNCTIONS:
bool	Channel::addClientChannel( int clientSocket ){
	if( static_cast<unsigned>(this->_maxUser) >= this->_user.size() )
	{
		this->_user.push_back( clientSocket );
		return( true );
	}
	else
		std::cout << "No more user" << std::endl;
	return( false );
}

void	Channel::removeClientChannel( int userSocket ){

	//Supprime l'ircOps du canal:
	if( !this->_ircOps.empty() )
	{
		for(std::vector<int>::iterator it = this->_ircOps.begin(); it != this->_ircOps.end(); it++)
		{
			if( *it == userSocket )
			{
				this->_ircOps.erase( it );
				--it;
				std::cout << "IrcOps socket [" << userSocket << "] erased" << std::endl;
			}
		}
	}

	// Supprime le user du canal:
	if( !this->_user.empty() )
	{
		for(std::vector<int>::iterator it = this->_user.begin(); it != this->_user.end(); it++)
		{
			if( *it == userSocket )
			{
				this->_user.erase( it );
				--it;
				std::cout << "User socket [" << userSocket << "] erased" << std::endl;
			}
		}
	}
}

void		Channel::setUserInvite ( int socketInvite, int flag ){
	if(flag == PUSH)
		this->_userInvitation.push_back(socketInvite);
	else{
		std::vector<int>::iterator it = find(this->_userInvitation.begin(), this->_userInvitation.end(), socketInvite);
		if (it == this->_userInvitation.end())
			return;
		this->_userInvitation.erase(it);
	}
}
