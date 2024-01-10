#include "Client.hpp"

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
std::string	enterNickname(int clientSocket){

	char buf[4096];

	while(1) {

		memset(buf, 0, 4096);
		int bytesRecv = recv(clientSocket, buf, 4096, 0);

		if (std::string(buf, 0, bytesRecv).find("NICK ") == 0)
		{
			buf[strlen(buf)-1] = '\0';
			newClient.setNick(buf + 5); //! set nick
			i = 1;
		}
			}

	// afficher tout 
	//	newClient.showInfo();
}

std::string	enterNickname(int clientSocket){
	while (1){
		if (std::string(buf, 0, bytesRecv).find("USER ") == 0){
			std::string 	name = substr(std::string(buf, 0, bytesRecv)
			break;
		}
	}
	return name;
}
//PASS
void	pass(int clientSocket, Server server){

	char buf[4096];

	int bytesRecv = recv(clientSocket, buf, 4096, 0);

	if (std::string(buf, 0, bytesRecv).find("PASS ") == 0 && 
			std::string(buf, 0, bytesRecv).find(server.getPassword()) == 5)
	{
		std::cout << clientSocket << " : good password" << std::endl;
		this->connected = true;
	}
	
}
