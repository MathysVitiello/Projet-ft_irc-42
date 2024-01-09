/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/09 16:51:29 by alamizan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"

int main(int argc, char **argv)
{
	try
	{
		// connfd: clientFd
		int connfd, sockfd;
		ssize_t				n; //size of buffer.
		char				buf[4096];
		struct sockaddr_in		clientaddr; 
		socklen_t			clientLen;                 

		// ------------------------------------------------------------- //
		// [1] Gestion arguments:
		checkArgs( argc, argv );

		// ------------------------------------------------------------- //
		// [2] Creer un serveur:
		Server server( atoi(argv[1]), argv[2] );

		// le configurer et le mettre en attente:
		// server.config();
		if (bind(server.getSocket(), (struct sockaddr *)&server.getAddr(),
				sizeof(server.getAddr())) < 0)
			throw std::runtime_error( "Can't bind to IP/port." );

		if (listen(server.getSocket(), SOMAXCONN) < 0)
			throw std::runtime_error( "Can't listen, or too many clients to handle." );

		// ------------------------------------------------------------- //
		// [3] integre le serveur dans une liste de "fd":
		// fd_set: c est une structure de données qui contient un ensemble 
		// de descripteurs d'archives:
		fd_set	master;

		// FD_ZERO permet de supprimer tous les bits d une structure fd_set.
		FD_ZERO(&master);
		// Integre ce nouvel ensemble de donnees dans le descripteur d archive fd_set master/
		//i.e server.getFd() is available for connections.
		FD_SET(server.getSocket(), &master);

		// ------------------------------------------------------------- //
		// [4] Boucle du serveur:
		for(;;)
		{
			fd_set	copy = master;
			if(select(1024, &copy, NULL, NULL, NULL) < 0)
				throw std::runtime_error( "Error in select" );

			// ------------------------------------------------------------- //
			/* this is done for new connections */
			if(FD_ISSET(server.getSocket(), &copy))   /* new client has requested connection */
			{
				// [5] Ajout de clients:
				clientLen = sizeof(clientaddr);
				if((connfd = accept(server.getSocket(), (struct sockaddr *)&clientaddr, 
								&clientLen)) == -1)
					throw std::runtime_error( "Problem with client connecting" );
				else
				{
					server.addClient(connfd, clientaddr);
					std::cout << server << std::endl;
				}
				FD_SET(connfd, &master); /* add the new file descriptor to set */
			}

			/* handle all the clients requesting */
			// remplacer it par client
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
					{
						buf[n] = '\0';
						if (std::string(buf, 0, n).find("quit") == 0)
							exit(1);
						std::cout << "client " << it->getId();
						std::cout << ": message a transmettre: " << buf << std::endl;                
					}
				}
			}   
		}

	close(server.getSocket());
    }
	catch ( std::exception & e )
	{
		std::cerr << CRED << "ERROR: " << NC << BRED << e.what() << NC << std::endl;
		return( 127 );
	}
    return 0;
		}
