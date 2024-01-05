#ifndef SERVER_HPP
#define SERVER_HPP

#include "library.hpp"

class Server{
	 public:
		Server();
		~Server();
	 private:
		std::vector<Chanel *>	chanel;
		std::vector<Client *>	client;
};

#endif
