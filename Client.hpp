#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "library.hpp"

class Client{
	 public:
		Client();
		~Client();
	 private:
		int			idClient;
		std::string name;
		std::string nickname;
};

#endif
