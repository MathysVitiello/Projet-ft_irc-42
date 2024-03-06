#include "Server.hpp"

// Displays all data included in the server:
// - std::cout << server << std::endl;
std::ostream & operator<<( std::ostream & o, Server const & src )
{
	std::vector<Client>::const_iterator it;

	std::cout << BPURPLE << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	o << "Server port: " << src.getPort() << std::endl;
	o << "Server reseau: " << src.getAddr().sin_port<< std::endl;
	o << "Server password: " << src.getPassword() << std::endl;
	o << "      ------------" << std::endl;

	// Displays all server clients and their information:
	for(it = src.getClients().begin(); it != src.getClients().end(); it++)
	{
		o << "		- socket client: " << it->getSocket() << std::endl;
		o << "		- addresse client: " << it->getAddr().sin_port << std::endl;
		o << "		- name client: " << it->getName() << std::endl;
		o << "		- nickname client: " << it->getNickname() << std::endl;
		o << "		- password: " << it->getConnect() << std::endl;
		o << "	  	  ------------" << std::endl << std::endl;
	}

	// Displays all customers on all channels:
	std::vector<Channel>::const_iterator itChannel;
	for(itChannel = src.getChannels().begin() ; itChannel != src.getChannels().end(); itChannel++)
	{
		std::cout << "- Channel: " << itChannel->getName() << std::endl;
		std::cout << "- password: " << itChannel->getPasswd() << std::endl;
		std::cout << "- pwd: " << itChannel->getPwd() << std::endl;
		std::cout << "- socket owner: " << itChannel->getOwner() << std::endl;
		std::cout << "- topic name: " << itChannel->getTopicName() << std::endl;
		std::cout << "- topic restricted privilege: " << itChannel->getTopicPrivilege() << std::endl;
		std::cout << "- invation: " << itChannel->getInvitation() << std::endl;
		std::cout << "- max user:" << itChannel->getMaxUser() << std::endl;
		for( unsigned i = 0; i < itChannel->getIrcOps().size(); i++ )
			o << "- socket ircOps: " << itChannel->getIrcOps()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUser().size(); i++ )
			o << "- socket user: " << itChannel->getUser()[i] << std::endl;
		for( unsigned i = 0; i < itChannel->getUserInvite().size(); i++ )
			o << "- user invitated: " << itChannel->getUserInvite()[i] << std::endl;
		o << "------------" << std::endl << std::endl;
	}
	std::cout << "-------------------------------------------"<< NC << std::endl;
	return( o );
}