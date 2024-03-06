#ifndef UTILS_HPP
#define UTILS_HPP

#include "library.hpp"

std::string	trimSpace(std::string str);
void	    checkArgs(int argc, char **argv);
int         indexFirstArg( std::string str );
int 		checkNickname(std::vector<Client> clients, Client *user, std::string channel);

#endif
