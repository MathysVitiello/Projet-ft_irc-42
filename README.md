# PROJET IRC
> [!IMPORTANT]
> Creation d'un serveur IRC en C++ capable de faire communiquer plusieurs clients localement.

<details>
<summary>Règles générales</summary>

- Votre programme ne doit en aucun cas se bloquer (même s'il manque de mémoire).
- Il ne doit pas s'arrêter de manière inattendue.
- Si cela se produit, votre projet sera considéré comme non fonctionnel
votre note sera de 0.
- Vous devez fournir un Makefile qui compilera vos fichiers sources. Il ne doit pas
relink.
- Votre Makefile doit au moins contenir les règles suivantes :
```
	- $(NAME), all, clean, fclean et re.
```
- Compilez votre code avec c++ et les drapeaux -Wall -Wextra -Werror
- Votre code doit être conforme à la norme C++ 98.
- Essayez de toujours développer en utilisant le plus de fonctionnalités C++ possible.
Par exemple, choisissez:
```
- <cstring> plutôt que <string.h>
``` 
- Les bibliothèques externes et les bibliothèques Boost sont interdites.
</details>

<details>
<summary>Serveur IRC</summary>
	
- ***Internet Relay Chat*** (IRC ; en français : « discussion relayée par Internet ») est un protocole de communication textuel sur Internet. Il sert à la communication instantanée principalement sous la forme de discussions en groupe par l’intermédiaire de canaux de discussion.
Il peut par ailleurs être utilisé pour faire du transfert de fichier.
- IRC est un protocole utilisant TCP et optionelle TLS.

1. **Aspect techniques:**
- Un serveur IRC peut se connecter à d'autres serveur IRC.
- Un réseaux IRC est un ensemble de serveur connectés l'un à l'autre.
- L'utilisateur utilise un logiciel IRC afin de se connecter à un des serveurs du réseaux.
- Le protocole ouvert est décrit par un RFC:
	- ***Requests for comments*** ("demandes de commentaires").
   	- documents décrivant les aspects et spécification techniques d'internet.

> RFC 28102 à RFC 2813

- Un serveur IRC est gérer par un ou plusieurs IrcOps (***IRC OPERATOR***):
  	- Ils sont nommés par les administrateurs.

2. **Les canaux:**
- ***Channel*** ("canal"):
	- Element de base pour communiquer sur un reseaux:
  	- Un channel est définit par une liste d'utilisateurs connectés à celui-ci.
- Pour rentrer dans un channel ou le créer:
```
join
```
> Les canaux pouvant etre vus de tous sur le réseau sont préfixée par #.
> 
> Les canaux peuvent etres locaux: 1 seul serveur et préfixée par &.

3. **Les modes:**
- Options positionnée sur des canaux ou des utilisateurs.
- affecte leurs modes de fonctionnement, leurs privilèges et leurs intéractions.

4. **Principales commandes:**
- Les commandes IRC sont toujours définiespar le caractère '/' placé en début de ligne.
- Elle est envoyée comme message au canal actif.
- Par exemple pur joindre un canal:
```
/join#canal
```
> Rejoint un canal public nommée canal.

6. **Bot IRC:**

8. **Les réseaux:**

</details>

<details>
<summary> Protocole TCP</summary>
	
-  ***Transmission Control Protocol*** (littéralement, « protocole de contrôle de transmissions »), abrégé TCP, est un protocole de transport fiable, en mode connecté.
</details>

<details>
<summary>protocole DCC</summary>
	
- ***Direct Client-to-Client***, protocole utilisé par de nombreux clients IRC qui permet une connections direct entre utilisateurs:
	- permet d envoyer des fichiers.
 	- permet de chatter plus rapidement et de maniere plus securise avec un autre utilisateur.
</details>

# Partie obligatoire
### Nom du programme:
	- ircserv
### Fichiers:
	- Makefile -> NAME, all, clean, fclean, re
	- *.{h, hpp}
	- *.cpp
	- *.tpp
	- fichiers de configuration optionnels
### Arguments:
	- port: le port d'écoute.
	- password: the connection password.
## Fonctions utilisables:
- Toutes en C++98.
<details>
	
<summary>Recommandées:</summary>

<details>
<summary>socket()</summary>

```cpp
int socket(int family, int type, int protocol);
```
	- family: famille du socket: AF_INET pour un socket IPv4.
	- type: specifie le type de socket, SOCK_STREAM pour TCP 
	- protocol: definit le protcole, IPPROTO_TCP pour un socket TCP.
>retourne le socket ou -1 en cas d'erreur.
</details>

- close
- setsockopt
- getsockname
- getprotobyname
- gethostbyname
- getaddrinfo
- freeaddrinfo
<details>
<summary>bind(): assigne une adresse locale à un socket</summary>
	
```cpp
int bind(int sckt, const struct addr* name, int namelen);
```
	- sckt est le socket auquel est assigné l'adresse.
	- name est la structure à assigner au socket.
	- namelen est la taille de cette structure.
</details>

- connect
<details>
<summary>listen(): permet au socket d'ecouter les connexions entrantes</summary>
	
```cpp
int listen(int sckt, int backlog);
```
	- sckt est le socket auquel les clients peuvent se connecter.
	- backlog est le nombres de connexions pouvant etres gérer.
 </details>
 
- accept
- htons
- htonl
- ntohs
- ntohl
- inet_addr
- inet_ntoa
- send
<details>
<summary>recv(): réceptionne des données sur le socket placé en paramètre.</summary>
	
```cpp
int recv(int socket, void* buffer, size_t len, int flags);
```
 	- socket: socket duquel receptionner le données.
  	- buffer: tampon où stocker les données.
	- len: nombres maximal d'octet a réceptionner.
 	- flag: masque d'option. Généralement 0.
```cpp
int socket;
// initialisation et connexion
char buffer[1024];
if (recv(socket, buffer, 1024, 0) <= 0)
        	// erreur ou connexion fermée
```
> Retourne le nombre d'octets reçus et stockés dans buffer.
> 
> Peut retourner 0 si la connexion a été terminée. Retourne -1 en cas d'erreur.
</details>

- signal
- sigaction
- lseek
- fstat
- fcntl
- poll
</details>

> [!NOTE]
> Ne pas hésiter à regarder les fonctions ci-dessus !

## Description:

• Un serveur IRC en C++98:

	- Pas besoin de développer un client.
	- Ne pas gérer la communication de serveur à serveur.

• L'exécutable sera exécuté comme suit :
```cpp
./ircserv <port> <password>
```
• port:

	- Le numéro de port sur lequel le serveur IRC écoutera les connexions IRC entrantes.
• password:

	- Le mot de passe de connexion.
 	- Il sera nécessaire à tout client IRC qui essaiera de se connecter au serveur.
> [!NOTE]
> Même si poll() est mentionné dans le projet, utiliser un équivalent tel que select(),
>  kqueue() ou epoll() est permit.

## Exigences:

- Le serveur doit être capable de gérer plusieurs clients en même temps et de ne jamais se bloquer.
- fork() n'est pas autorisée. Toutes les opérations d'E/S doivent être non bloquantes.
- Un seul poll() (ou équivalent) peut être utilisé pour gérer toutes ces opérations (lecture, écriture, mais aussi écoute, etc...).

>  [!CAUTION]
> Utiliser des descripteurs de fichiers non bloquants, il est possible d'utiliser des fonctions de lecture/récupération ou
>  d'écriture/envoi sans poll() (ou équivalent), et le serveur ne doit pas etre bloquant.
>
> Mais il consommera plus de ressources système.
> 
> Lire/recourir ou d'écrire/envoyer dans n'importe quel descripteur de fichier
> sans utiliser poll() (ou équivalent) ne valide pas le projet !

- Il existe plusieurs clients IRC. Choisir l'un d'entre eux comme référence:
	- Le client de référence sera utilisé pendant la présentation.
- Le client de référence doit pouvoir se connecter à au serveur sans rencontrer d'erreur.
- La communication entre le client et le serveur doit se faire via TCP/IP (v4 ou v6).
- L'utilisation du client de référence avec le serveur doit être similaire à son utilisation avec n'importe quel serveur IRC officiel.
Cependant, implémenter les fonctionnalités suivantes :
	- être en mesure:
 		- de s'authentifier
		- de définir un pseudonyme (changeable a tout moment)
		- un nom d'utilisateur (inchangeable)
		- de rejoindre un canal
		- envoyer et recevoir des messages privés en utilisant votre client de référence.
	- Tous les messages envoyés par un client à un canal doivent être transmis à tous les autres clients qui ont rejoint le canal.
 	- Vous devez avoir des opérateurs et des utilisateurs réguliers.
  	- Ensuite, vous devez mettre en œuvre les commandes spécifiques aux opérateurs de canaux
des opérateurs:
		- KICK   -> Ejecter un client du canal.
  		- INVITE -> Inviter un client à un canal.
  	 	- TOPIC  -> Modifier ou afficher le thème du canal.
  	  	- MODE   -> Changer le mode du canal :
  
| Commande en MODE | Description |
| --- | --- |
| i | Définir/supprimer un canal sur invitation uniquement |
| t | Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux |
| k | Définir/supprimer la clé de canal (mot de passe) |
| o | Donner/reprendre le privilège de l'opérateur du canal |
| l | Définir/supprimer la limite d'utilisateurs pour le canal |

## Pour MacOS seulement:

> [!IMPORTANT]
> Comme MacOS n'implémente pas write() de la même manière que les autres systèmes d'exploitation Unix, vous êtes autorisé à utiliser fcntl().
>
> Vous devez utiliser les descripteurs de fichiers en mode non bloquant afin d'obtenir un
> comportement similaire à celui des autres systèmes d'exploitation Unix.

> [!CAUTION]
> Cependant, vous ne pouvez utiliser fcntl() que de la manière suivante, tout autre drapeau est interdit :
```cpp
fcntl(fd, F_SETFL, O_NONBLOCK) ;
```

## Exemple test:
- Vérifier absolument toutes les erreurs et tous les problèmes possibles (réception de données partielles, faible largeur de bande, etc...).
- Pour s'assurer que votre serveur traite correctement tout ce que vous lui envoyez, vous pouvez effectuer le test simple suivant en utilisant nc:
```
\$> nc 127.0.0.1 6667
com^Dman^Dd
\$>
```

- Utilisez ctrl+D pour envoyer la commande en plusieurs parties : 'com', puis 'man', puis 'd\n'.
- Pour traiter une commande, il faut d'abord agréger les paquets reçus afin de la reconstruire.

### listen:
```cpp
int listen(SOCKET sckt, int backlog);
```
- Permet au socket d'ecouter les connexions entrantes
	- sckt est le socket auquel les clients peuvent se connecter.
	- backlog est le nombres de connexions pouvant etres gérer.
 
# PARTIE BONUS

- Voici les fonctions supplémentaires que vous pouvez ajouter à votre serveur IRC pour qu'il ressemble encore plus à un serveur IRC réel.
véritable serveur IRC :
	- Gérer le transfert de fichiers.
	- Un bot.

> [!CAUTION]
> La partie bonus ne sera évaluée que si la partie obligatoire est PARFAITE. Parfaite signifie que la partie > obligatoire a été intégralement réalisée et fonctionne sans dysfonctionnement. Si vous n'avez pas > satisfait à TOUTES les exigences obligatoires, votre partie bonus ne sera pas évaluée du tout !
