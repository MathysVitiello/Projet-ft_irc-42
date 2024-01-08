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

//NICK USER
void NICK_USER(int clientSocket, Client newClient){

    char buf[4096];
    int i = 0;
    int j = 0;

    while(i != 1 || j != 1) {
        
        memset(buf, 0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0);

        if (std::string(buf, 0, bytesRecv).find("NICK ") == 0)
        {
            buf[strlen(buf)-1] = '\0';
            newClient.setNick(buf + 5); //! set nick
            i = 1;
        }
        if (std::string(buf, 0, bytesRecv).find("USER ") == 0)
        {
            buf[strlen(buf)-1] = '\0';
            newClient.setName(buf + 5); //! set name            
            j = 1;
        }
    }

    // afficher tout 
//	newClient.showInfo();
}

//PASS
// void PASS(int clientSocket, char *argv){
// 
    // char buf[4096];
    // int i = 0;
    // 
    // Client newClient; //! le declarer ailleurs, plus tot, a chque tentative de connexion / dans le main
// 
    // while(i == 0) {
        // int bytesRecv = recv(clientSocket, buf, 4096, 0);
        // condition pour le mdp = "PASS mdp" avant d'avoir acces au server
        // if (std::string(buf, 0, bytesRecv).find("PASS ") == 0 && 
            // std::string(buf, 0, bytesRecv).find(argv) == 5)
        // {
            // std::cout << clientSocket << " is now connected" << std::endl;
            // newClient.setId(clientSocket); //! set id
            // i = 1;
        // }
    // }
    // NICK_USER(clientSocket, newClient);
// }
