/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/17 15:24:27 by alamizan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "library.hpp"
int exitFlag = 0;

void	sigHandler( int signum )
{
	exitFlag = signum;
	throw std::runtime_error( "" );
}

int main(int argc, char **argv)
{
	try
	{
		int newSocket, sockfd, nbFds;
		ssize_t				sizeRead; //size of buffer.
		char 				buf[4096];
		struct sockaddr_in	clientaddr; 
		socklen_t			clientLen;                 

		// ------------------------------------------------------------- //
		// [1] Gestion arguments:
		checkArgs( argc, argv );

		// ------------------------------------------------------------- //
		// [2] Creer un serveur et le configurer:
		Server server( atoi(argv[1]), argv[2] );

		// ------------------------------------------------------------- //
		// [3] integre le serveur dans une liste de "fd":
		// fd_set: c est une structure de données qui contient un ensemble 
		// de descripteurs d'archives:
		fd_set	master;
		

		// FD_ZERO permet de supprimer tous les bits d une structure fd_set.
		FD_ZERO(&master);
		// Integre ce nouvel ensemble de donnees dans le descripteur d archive fd_set master/
		FD_SET(server.getSocket(), &master);

		// ------------------------------------------------------------- //
		// [4] Boucle du serveur:

		// Gestion des signaux:
		signal( SIGQUIT, SIG_IGN );
		signal( SIGINT, sigHandler );

		while ( 1 )
		{
			std::cout << server << std::endl;
			fd_set	copy = master;
			if( ( nbFds = select(1024, &copy, NULL, NULL, NULL) ) < 0 )
				throw std::runtime_error( "Error in select" );

			// ------------------------------------------------------------- //
			/* this is done for new connections */
			if( FD_ISSET(server.getSocket(), &copy) )   /* new client has requested connection */
			{
				// [5] Ajout de clients:
				clientLen = sizeof(clientaddr);
				if( (newSocket = accept(server.getSocket(), (struct sockaddr *)&clientaddr, 
								&clientLen)) == -1 )
					throw std::runtime_error( "Problem with client connecting" );

				server.addClient(newSocket, clientaddr);
				FD_SET(newSocket, &master); /* add the new file descriptor to set */
			}

			/* handle all the clients requesting */
			for( size_t i = 0; i < server.getClients().size(); i++ )
			{
				if( (sockfd = server.getClients()[i].getSocket()) < 0 )
					continue;

				if( FD_ISSET(sockfd, &copy) )
				{
					sizeRead = recv(sockfd, buf, sizeof(buf), 0);
					if ( sizeRead <= 0 )
					{
						/* connection closed by client side */
						if ( sizeRead == 0 )
							std::cerr << "Connection socket's client " << server.getClients()[i].getSocket() << " closed by server" << std::endl;
						else
							perror("recv");
						close(sockfd);
						FD_CLR(sockfd, &master);
						server.removeClient( i );
					}
					else{
						buf[sizeRead - 1] = '\0';
						server.setClients( buf, i ); //. SplitCmd( buf );
						server.command(i);
					}
					if(--nbFds < 0)
						break;
				}
			}   
		}
		int al = server.getSocket();
		close( al );
    }
	catch ( std::exception & e )
	{
		if( exitFlag != SIGINT )
			std::cerr << CRED << "ERROR: " << NC << BRED << e.what() << NC << std::endl;

		if( exitFlag == SIGINT )
			std::cerr << BPURPLE << "Fermeture du serveur et deconnexion des clients !! " << NC << std::endl;

		std::cerr << BRED << "Code erreur = " << exitFlag << std::endl;
		std::cerr << "Renvoi valgrind: ( valgrind --track-origins=yes --leak-check=full --track-fds=yes ./ircserv arg1 arg2 )"  << std::endl;
		std::cerr << BGREEN;
		return( exitFlag );
	}
    return ( 0 );
}
