/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/06 14:20:58 by alamizan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"
#include <stdexcept>

//!https://www.youtube.com/watch?v=cNdlrbZSkyQ

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw( std::runtime_error("Not the right number of arguments") );

		// ------------------------------------------------------------------------ //
		// 1. Create a socket
		// AF_INET = famille d’adresses pour IPv4.
		// SOCK_STREAM is a connection-based protocol.
		// La connection est etablie et les deux personnes ont une conv jusqua ce que
		// la connection soit arrete par une des deux personnes ou un erreur de network.
		int listening = socket(AF_INET, SOCK_STREAM, 0);
		if (listening == -1)
			throw( std::runtime_error("Can't create a socket !") );

		// ------------------------------------------------------------------------ //
		// 2.Joindre socket a IP adress / port
		sockaddr_in hint; // structure 
		hint.sin_family = AF_INET;
		hint.sin_port = htons(atoi(argv[1])); 

		// 0.0.0.0 veut dire que cest nimporte quelle adresse
		// Cette fonction convertit la chaîne de caractères src en une structure
		// d'adresse réseau de la famille af puis copie cette structure dans dst.
		inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
		if (bind(listening, (sockaddr *) &hint, sizeof(hint)) == -1)
			throw( std::runtime_error("Can't bind to IP/port !") );


		// ------------------------------------------------------------------------ //
		// 3. Mark the socket for listening in
		// listen marque la socket comme une socket passive
		// une socket qui sera utilisée pour accepter les demandes de connexions
		// entrantes en utilisant accept().
		if (listen(listening, SOMAXCONN) == -1)
			throw( std::runtime_error("Can't listen !") );

		// Accept a call
		sockaddr_in client; // structure 
		socklen_t clientSize = sizeof(client); //! a voir si on a le droit de socklen_t
		char host[NI_MAXHOST]; //max 1025
		char service[NI_MAXSERV]; //max 32


		int clientSocket = accept(listening, (sockaddr *) &client, &clientSize);
		if (clientSocket == -1)
		{
			std::cerr << "Problem with client connecting";
			return 3;
		}

		close(listening); // parce que listening est un fd.

		memset(host, 0, NI_MAXHOST);
		memset(service, 0, NI_MAXSERV);

		int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
		if (result)
		{
			std::cout << host << " connected on " << service << std::endl;
		}
		else 
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
		}

		//While receiving display message
		char buf[4096];
		std::string input;

		//! ici test de mdp, va faloir le fair epour chauqe client, un int qui indique si il sont connecte ou non 
		PASS(clientSocket, argv[2]);

		while(true){

			memset(buf, 0, 4096);
			int bytesRecv = recv(clientSocket, buf, 4096, 0);
			if (bytesRecv == -1)
			{
				std::cerr << "There was a connetion issue" << std::endl;
				break;
			}
			if (bytesRecv == 0)
			{
				std::cerr << "The client disconnected" << std::endl;
				break;
			}

			std::cout << std::string(buf, 0, bytesRecv);
			send(clientSocket, buf, bytesRecv + 1, 0);
		}

		close(clientSocket);
	}
	catch ( std::exception & e )
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return( 127 );
	}
    return 0;
}


/*
socket = endpoint of communication ->  You make a call to the socket() system routine. It returns the socket descriptor,
and you communicate through it using the specialized send() and recv() (man send, man recv) socket calls.
*/


/*
In order to properly support multiple connections you should fire up
a new thread for each incoming connection.
Each new connection is identified by its
own unique socket descriptor returned by accept(). A simple example:
*/