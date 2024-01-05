/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/04 11:44:22 by mvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <string>

//!https://www.youtube.com/watch?v=cNdlrbZSkyQ
 
int main(void){

    //1. Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = famille d’adresses pour IPv4. | SOCK_STREAM is a connection-based protocol. La connection est etablie et les deux personnes ont une conv jusqua ce que la connection soit arrete par une des deux personnes ou un erreur de network.
    if (listening == -1)
    {
        std::cerr << "Can't create a socket !";
        return 1;
    }




    //2.Joindre socket a IP adress / port
    sockaddr_in hint; // structure 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); //== host to network short, permet de convertir le int en network byte -> c'est le port
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // | 0.0.0.0 veut dire que cest nimporte quelle adresse | Cette fonction convertit la chaîne de caractères src en une structure d'adresse réseau de la famille af, puis copie cette structure dans dst. 

    if (bind(listening, (sockaddr *) &hint, sizeof(hint)) == -1)
    {
        std::cerr << "Can't bind to IP/port !";
        return 2;
    }



    //3. Mark the socket for listening in

    //listen marque la socket comme une socket passive, c'est-à-dire comme une socket qui sera utilisée pour accepter les demandes de connexions entrantes en utilisant accept().
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen !";
        return 3;
    }

    //Accept a call
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

        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
        send(clientSocket, buf, bytesRecv + 1, 0);
    }

    close(clientSocket);
    return 0;
}



/*
socket = endpoint of communication ->  You make a call to the socket() system routine. It returns the socket descriptor,
and you communicate through it using the specialized send() and recv() (man send, man recv) socket calls.
*/