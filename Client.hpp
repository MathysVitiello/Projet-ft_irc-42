#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "library.hpp"

class Client{
	 public:
		Client();
		~Client();
	 private:
		int			const & idClient;
		std::string const & name;
		std::string nickname;
};

#endif
