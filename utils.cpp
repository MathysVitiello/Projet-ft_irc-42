#include "utils.hpp"

std::string	trimSpace(std::string str){
	for (size_t i = 0; i < str.size() && str[i] == ' '; i++){
		str.erase(i, i + 1);
		i = -1; 
	}
	return str;	
}

void	checkArgs(int argc, char **argv)
{
	if (argc != 3)
			throw std::runtime_error( "Not the right number of arguments" ) ;

	std::string    nb = argv[1];
	for (unsigned int i = 0; i < nb.length(); i++)
		if (!isdigit (nb[i]))
			throw std::runtime_error( "the First argument is the port, pls write a number!" );
	if (std::string(argv[2]).length() == 0)
        throw std::runtime_error("write password, pls.");
}

// ServerMode.cpp for modePrivilege (o):
int checkNickname(std::vector<Client> clients, Client *user, std::string channel)
{
	int flag 			= 0;
	int index 			= 0;
	std::string nick 	= user->getNickname();

	if( user->getCmdBuf().size() != 3 )
	{
		send(user->getSocket(), ERR_NEEDMOREPARAMS(nick, "MODE").c_str(),
				ERR_NEEDMOREPARAMS(nick, "MODE").size(), 0);
		return( -1 );
	}

	std::vector<Client>::const_iterator it = clients.begin();
	for (; it != clients.end(); it++, index++)
	{
		if( it->getNickname() == nick )
		{
			flag = 1;
			break;
		}
	}
	if( flag == 0 )
	{
		send(user->getSocket(), ERR_NOSUCHNICK(nick, channel).c_str(), 
			ERR_NOSUCHNICK(nick, channel).size(), 0);
		return( -1 );
	}
	int socketClient = clients[index].getSocket();
	return( socketClient );
}


