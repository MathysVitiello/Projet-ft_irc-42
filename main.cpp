/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/08 17:23:56 by alamizan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

//!https://www.youtube.com/watch?v=cNdlrbZSkyQ

int main(int argc, char **argv)
{
	try
	{
		// ------------------------------------------------------------- //
		// [1] Gestion arguments:
		if (argc != 3)
			throw std::runtime_error( "Not the right number of arguments" ) ;

		std::string    nb = argv[1];
		for (unsigned int i = 0; i < nb.length(); i++)
			if (!isdigit (nb[i]))
				throw std::runtime_error("the First argument is the port, pls write a number!");

		// ------------------------------------------------------------- //
		// [2] Creation d un socket, parametre:
		Server server( atoi(argv[1]), argv[2] );

		// ------------------------------------------------------------- //
		// [3] Creer un serveur:
		int ret = bind(server.getFd(), (sockaddr*)&server.getAddr(), sizeof(server.getAddr()));
		if( ret != 0 )
			throw std::runtime_error( "Can't bind to IP/port." );

		ret = listen( server.getFd(), SOMAXCONN );
		if( ret != 0 )
			throw std::runtime_error("Can't listen." );
		std::cout << "---------------------------" << std::endl;
		std::cout << "Serveur en route ..." << std::endl;
		std::cout << "---------------------------" << std::endl;

		// ------------------------------------------------------------- //
		// [4] Ajout de clients:
		for (;;)
		{
			sockaddr_in port;
			socklen_t addrlen = sizeof(server.getAddr());
			int newClient = accept(server.getFd(), (sockaddr*)(&port), &addrlen);
			if (newClient != -1)
			{
				server.addClient(newClient, port);
				std::cout << "Connexion de " << port.sin_port << std::endl;
				std::cout << server << std::endl;
			}
			else
				break;
		}

		// Ferme le socket:
		close( server.getFd() );
	}
	catch ( std::exception & e )
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return( 127 );
	}
	return 0;
}
