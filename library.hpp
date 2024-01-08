#ifndef LIBRARY_HPP
// -------------------- DEFINE ---------------------- //
# define          LIBRARY_HPP

# define CRED		  "\033[6;91m"
# define BRED		  "\033[1;91m"
# define BPURPLE  "\001\e[1;95m\002"
# define NC			  "\001\e[0m\002"

// --------------------  LIBRARY --------------------- //
# include <unistd.h>
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <vector>
# include <algorithm>    
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <functional>   
# include <errno.h>

// --------------------- CLASS ---------------------- //
# include "Client.hpp"
# include "Server.hpp"
//#include "Channel.hpp"

//PassNickUser.cpp
void PASS(int clientSocket, char *argv);

#endif
