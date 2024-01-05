/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pswd_policy.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:44:21 by mvitiell          #+#    #+#             */
/*   Updated: 2024/01/04 11:44:22 by mvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.hpp"


void password_test(int clientSocket, char *argv){

    char buf[4096];
    int i = 0;

    while(i == 0) {

        int bytesRecv = recv(clientSocket, buf, 4096, 0);

        if (std::string(buf, 0, bytesRecv) == "PASS\n")
        {
            std::cout << "Is trying to connect : " << std::endl;
            while(i == 0){

                memset(buf, 0, 4096);
                int bytesRecv = recv(clientSocket, buf, 4096, 0);

                if (std::string(buf, 0, bytesRecv - 1) == argv)
                {
                    std::cout << "Good Password" << std::endl;
                    i = 1;
                    break;
                }
                else 
                    std::cout << "Wrong Password" << std::endl;
            }
        }
    }
}

//! checker quil soit pas vide le mdp