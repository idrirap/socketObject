#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <mutex>
#include <vector>

/*  
** Codé par la Idris Corporation
*/


class SocketObjet{
    private:
        int socket_;                                        // socket principal.
        std::vector<int> socketfd_;                         // sockets secondaires qui sont créés lors d'une connexion avec un client
        struct sockaddr_in their_addr;                      // structure contenant les information de connexion distante
        struct sockaddr_in my_addr;                         // structure contenant les information de connexion local
        int sin_size;
        int yes = 1;
        int backlog;                                        // valeur de connexion maximum en simultané. 10 par défaut
        std::mutex mtx;
    public:
        SocketObjet(int backlog);                           // initialiser avec le nombre de co max simultanés
        SocketObjet();                                      //
        ~SocketObjet();
        int getSocket(int id);
        std::vector<int> getSockets();
        int getMax();
        void setMyAddr(int port_);                          // Initialisation de la structure sockadr_in. Preciser le port local (utile uniquement pour les serveurs)
        void setTheirAddr(std::string addr, int port_);     // Initialisation de la structure sockadr_in. Preciser le port et l'adresse ip distante
        void envoyer(char *, int sockfd);                  
        void connection();                                  // Creation d'une connexion
        std::string recevoir(int tampon_,int sockfd);       // -1 pour la socket main
        void binder();                                      // Utiliser la fonction bind avec une socket ouverte sur l'exterieur (initialisation de socketfd_)
        void ecouter();                                     // Utiliser la fonction listen qui check si un client tente de se connecter
        int accepter();                                     // Accepter la connexion
        void fermer(int sockfd);   
        void envoyerGroup(char * c, std::vector<int> destinataires);  // envoie en broadcast un message à un vecteur de sockets                     
};