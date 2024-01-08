#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <vector>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>   
#include <algorithm>    
#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

/* our library  */
#include "Client.hpp"
//#include "Channel.hpp"
#include "Server.hpp"


//PassNickUser.cpp
void PASS(int clientSocket, char *argv);

#endif
