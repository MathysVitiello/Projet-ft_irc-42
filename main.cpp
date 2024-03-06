/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/03/06 14:47:13 by alamizan         ###   ########.fr       */
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
		// [1] Argument management
		checkArgs( argc, argv );

		// ------------------------------------------------------------- //
		// [2] Create server and configuration:
		Server server( atoi(argv[1]), argv[2] );

		// ------------------------------------------------------------- //
		// [3] Put server in "fd" list:
		// fd_set: data struct of fd
		fd_set	master;
		

		// FD_ZERO is able to delete all the bits from structure fd_set.
		FD_ZERO(&master);
		// Integrate this new dataset into fd_set master/
		FD_SET(server.getSocket(), &master);

		// ------------------------------------------------------------- //
		signal( SIGQUIT, SIG_IGN );
		signal( SIGINT, sigHandler );

		// ------------------------------------------------------------- //
		// [4] Server loop:

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
				// [5] Add clients:
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
					if ( (sizeof(buf) >= 4 && buf[0] == 'Q' && buf[1] == 'U' && buf[2] == 'I' && buf[3] == 'T')
						|| sizeRead <= 0 )
					{
						/* connection closed by client side */
						if ( sizeRead == 0 )
							std::cout << "Connection socket's client " << server.getClients()[i].getSocket() << " closed by server" << std::endl;
						else
							std::cout << "close" << std::endl;
						close(sockfd);
						FD_CLR(sockfd, &master);
						server.removeClient( i );
					}
					else{
						if (buf[sizeRead - 1] == '\n'){
							if (buf[sizeRead - 2] == '\r')
								buf[sizeRead - 2] = '\0';
							buf[sizeRead - 1] = '\0';
							std::string buffer = server.bufTmp(buf, PUSH, i);
							server.setClients( buffer, i ); //. SplitCmd( buf );
							server.command(i);
							server.bufTmp(buf, DEL, i);
						}
						else
							server.bufTmp(buf, PUSH, i);
						buf[sizeRead] = '\0';
						bzero(buf, 4096);
					}
					if(--nbFds < 0)
						break;
				}
			}   
		}
		int socket = server.getSocket();
		close( socket );
    }
	catch ( std::exception & e )
	{
		if( exitFlag != SIGINT )
			std::cerr << CRED << "ERROR: " << NC << BRED << e.what() << NC << std::endl;

		if( exitFlag == SIGINT )
			std::cerr << BPURPLE << "Fermeture du serveur et deconnexion des clients !! " << NC << std::endl;
		std::cerr << "Renvoi valgrind: ( valgrind --track-origins=yes --leak-check=full --track-fds=yes ./ircserv arg1 arg2 )"  << std::endl;
		return( exitFlag );
	}
    return ( 0 );
}
