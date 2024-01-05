#ifndef SERVER_HPP
#define SERVER_HPP

#include "library.hpp"

class Server{
	 public:
		Server();
		~Server();
	 private:
		std::string				passwd;
		unsigned int			port;	
		std::vector<Channel *>	channel;
		std::vector<Client *>	client;
};

#endif
