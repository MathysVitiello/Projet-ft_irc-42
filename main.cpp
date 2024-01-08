/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/08 22:20:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"

int main(int argc, char **argv)
{
	try
	{
		int connfd, sockfd, nready;
		ssize_t				n; //size of buffer.
		char				buf[4096];
		struct sockaddr_in	clientaddr; 
		fd_set				copy;
		socklen_t			clilen;                 

		// ------------------------------------------------------------- //
		// [1] Gestion arguments:
		checkArgs( argc, argv );

		// ------------------------------------------------------------- //
		// [2] Creer un serveur, le configurer et le mettre en attente:
		Server server( atoi(argv[1]), argv[2] );

		// server.config();
		if (bind(server.getFd(), (struct sockaddr *)&server.getAddr(),
				sizeof(server.getAddr())) < 0)
			throw std::runtime_error( "Can't bind to IP/port !" );

		if (listen(server.getFd(), SOMAXCONN) < 0)
			throw std::runtime_error( "ERROR on listen, or too many clients to handle." );
	
		// ------------------------------------------------------------- //
		// [3] integre le serveur dans une liste de "fd":
		// fd_set: c est une structure de données qui contient un ensemble 
		// de descripteurs d'archives:
		fd_set	master;

		// FD_ZERO permet de supprimer tous les bits d une structure fd_set.
		FD_ZERO(&master);
		// Integre ce nouvel ensemble de donnees dans le descripteur d archive fd_set master/
		//i.e server.getFd() is available for connections.
		FD_SET(server.getFd(), &master);

		// ------------------------------------------------------------- //
		// [3] Ajout de clients:
		for(;;)
		{
			copy = master;
			if((nready = select(SOMAXCONN, &copy, NULL, NULL, NULL)) < 0){
				throw std::runtime_error( "Error in select" );
			}

			// ------------------------------------------------------------- //
			/* this is done for new connections */
			if(FD_ISSET(server.getFd(), &copy))   /* new client has requested connection */
			{
				// socklen_t addrlen = sizeof(server.getAddr());
				clilen = sizeof(clientaddr);
				if((connfd = accept(server.getFd(), (struct sockaddr *)&clientaddr, 
								&clilen)) == -1)
					throw std::runtime_error( "accept error" );
				else
				{
					server.addClient(connfd, clientaddr);
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
						// il faut integrer le char buf[1024] pour chaque client
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
		std::cerr << CRED << "ERROR: " << NC << BRED << e.what() << NC << std::endl;
		return( 127 );
	}
    return 0;
}
