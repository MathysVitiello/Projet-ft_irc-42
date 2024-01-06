#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "library.hpp"

class Client{
	 public:
		Client();
		~Client();
		void setId( int id );
		void setName( std::string name );
		void setNick( std::string nick );
		void showInfo();
	 private:
		int			 idClient;
		std::string  name;
		std::string  nickname;
};

#endif
