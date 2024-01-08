/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/08 18:36:00 by alamizan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"
#include <stdexcept>
#include <sys/types.h>

//!https://www.youtube.com/watch?v=cNdlrbZSkyQ

int main(int argc, char **argv)
{
	try
	{
		int connfd, sockfd;                   
		
		int nready;
		ssize_t n; //size of buffer.


		fd_set copy, master;
		char buf[4096];
		
		socklen_t clilen;                 
		struct sockaddr_in  clientaddr; 
		// ------------------------------------------------------------- //
		// [1] Gestion arguments:
		if (argc != 3)
			throw std::runtime_error( "Not the right number of arguments" ) ;

		std::string    nb = argv[1];
		for (unsigned int i = 0; i < nb.length(); i++)
			if (!isdigit (nb[i]))
				throw std::runtime_error("the First argument is the port, pls write a number!");


		// ------------------------------------------------------------- //
		// [2] Creer un serveur:
		Server server( atoi(argv[1]), argv[2] );

		if (bind(server.getFd(), (struct sockaddr *)&server.getAddr(),
				sizeof(server.getAddr())) < 0)
			throw( std::runtime_error("ERROR on binding") );

		if (listen(server.getFd(), SOMAXCONN) < 0)
			throw( std::runtime_error("ERROR on listen, or too many clients to handle.") );
	
		// ------------------------------------------------------------- //
		// [4] Ajout de clients:
		FD_ZERO(&master);
		FD_SET(server.getFd(), &master); //i.e server.getFd() is available for connections.

		for(;;)
		{
			copy = master;
			if((nready = select(SOMAXCONN, &copy, NULL, NULL, NULL)) < 0){
				throw( std::runtime_error("Error in select") );
			}

			// ------------------------------------------------------------- //
			/* this is done for new connections */
			if(FD_ISSET(server.getFd(), &copy))   /* new client has requested connection */
			{
				// socklen_t addrlen = sizeof(server.getAddr());
				clilen = sizeof(clientaddr);
				if((connfd = accept(server.getFd(), (struct sockaddr *)&clientaddr, 
								&clilen)) == -1)
					throw( std::runtime_error("accept error") );
				else
				{
					server.addClient(connfd, clientaddr);
					//std::cout << "Connexion de " << server.sin_port << std::endl;
					std::cout << server << std::endl;
				}
				FD_SET(connfd, &master); /* add the new file descriptor to set */
			}

			/* handle all the clients requesting */
			std::vector<Client>::const_iterator it;

			for(it = server.getClients().begin(); it != server.getClients().end(); it++)
			{
				if((sockfd = it->getId()) < 0)
					continue;
				
				if(FD_ISSET(sockfd, &copy))
				{
					if( (n = recv(sockfd, buf, 4096, 0))==0 )
					{
						/* connection closed by client side */
						close(sockfd);
						FD_CLR(sockfd, &master);
						//client[i] = -1;
					}
					else
						std::cout << buf << std::endl;                
					if(--nready < 0)
						break;
				}
			}   
		}

	close(server.getFd());
    }
	catch ( std::exception & e )
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return( 127 );
	}
    return 0;
}
