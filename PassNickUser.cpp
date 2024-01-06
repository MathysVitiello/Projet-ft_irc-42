/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassNickUser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/04 11:44:22 by mvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.hpp"

/*
Une commande "PASS" n'est pas nécessaire pour établir une connexion, mais elle doit précéder
la combinaison suivante des messages NICK/USER.
Il est fortement recommandé que toutes les connexions de serveurs utilisent un mot de passe
afin de donner un niveau de sécurité satisfaisant aux connexions.
L'ordre des commandes recommandé pour l'enregistrement d'un client est le suivant :
1. Message PASS
2. Message NICK
3. Message USER
*/
//! les clients savent quils doivent faire ca

//NICK USER
void NICK_USER(int clientSocket){

    char buf[4096];
    int i = 0;
    int j = 0;

    while(i != 1 || j != 1) {
        int bytesRecv = recv(clientSocket, buf, 4096, 0);

        if (std::string(buf, 0, bytesRecv).find("NICK ") == 0)
        {
            std::cout << clientSocket << " le reste deviens le nickname, a faire" << std::endl;
            i = 1;
        }
        if (std::string(buf, 0, bytesRecv).find("USER ") == 0)
        {
            std::cout << clientSocket << " le reste deviens le USER, a faire" << std::endl;
            j = 1;
        }
    }
}

//PASS
void PASS(int clientSocket, char *argv){

    char buf[4096];
    int i = 0;

    while(i == 0) {
        int bytesRecv = recv(clientSocket, buf, 4096, 0);

        // condition pour le mdp = "PASS mdp" avant d'avoir acces au server
        if (std::string(buf, 0, bytesRecv).find("PASS ") == 0 && 
            std::string(buf, 0, bytesRecv).find(argv) == 5)
        {
            std::cout << clientSocket << " is now connected" << std::endl;
            i = 1;
        }
    }
    NICK_USER(clientSocket);
}
