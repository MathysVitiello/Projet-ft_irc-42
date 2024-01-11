#ifndef LIBRARY_HPP
# define LIBRARY_HPP

// -------------------- DEFINE COLOR ---------------------- //
# define CRED		"\033[6;91m"
# define BRED		"\033[1;91m"
# define BPURPLE 	"\001\e[1;95m\002"
# define NC			"\001\e[0m\002"

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
# include "./Client/Client.hpp"
# include "./Server/Server.hpp"
# include "./Channel/Channel.hpp"
# include "define.hpp"
# include "utils.hpp"


#endif
