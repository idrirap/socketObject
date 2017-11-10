#include <iostream>
#include <string>
#include <vector>
#include "socketObjet.h"

using namespace std;


SocketObjet::SocketObjet(){
    backlog=10;
    for(int i=0; i<backlog;i++){
        socketfd_[i]= -1;
    }
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ == -1){
        perror("Socket : ");
        exit(1);
    }
    if(setsockopt(socket_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
        perror("Set socket : ");
        exit(1);
    }
}

int SocketObjet::getSocket(int id){
    if(id < 0 ){
        return socket_;
    }
    else{
        return socketfd_[id];
    }
}

std::vector<int> SocketObjet::getSockets(){
    return socketfd_;
}

SocketObjet::SocketObjet(int BACKLOG){
    backlog=BACKLOG;
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ == -1){
        perror("Socket : ");
        exit(1);
    }
    if(setsockopt(socket_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
        perror("Set socket : ");
        exit(1);
    }
}

SocketObjet::~SocketObjet(){
    
}

void SocketObjet::setMyAddr(int port_){//pour un serveur
    my_addr.sin_addr.s_addr = INADDR_ANY;                               // adresse, devrait être converti en reseau mais est egal à 0
    my_addr.sin_family = AF_INET;                                       // type de la socket
    my_addr.sin_port = htons(port_);                                    // port, converti en reseau
    bzero(&(my_addr.sin_zero), 8);                                      // mise a zero du reste de la structure
}

void SocketObjet::setTheirAddr(string addr, int port_){
    their_addr.sin_addr.s_addr = inet_addr((char*)addr.c_str());        // ip
    their_addr.sin_family = AF_INET;                                    // type de la socket
    their_addr.sin_port = ntohs(port_);                                 // port, converti en reseau
    bzero(&(their_addr.sin_zero), 8);                                   // mise a zero du reste de la structure
}

void SocketObjet::envoyer(char * c, int sockfd){
    if(sockfd != -1){
        if(send(socketfd_[sockfd], c, strlen(c),0)==-1){
            perror("SEND : ");
            exit(1);
        }
    }else{
        if(send(socket_, c, strlen(c),0)==-1){
            perror("SEND : ");
            exit(1);
        }
    }
}

string SocketObjet::recevoir(int tampon_, int sockfd){
    int tmp;
    char* retour = new char[tampon_];
    if(sockfd != -1){
        tmp = recv(socketfd_[sockfd], retour, tampon_, 0);
    }else{
        tmp = recv(socket_, retour, tampon_, 0);
    }
    if (tmp==-1){ 
        perror("RECEVOIR : ");
        exit(1);
    }
    if(tmp >= tampon_){
        cout<<"ERREUR BILLY!!"<<endl;
        return NULL;
    }
    retour[tmp]='\0';
    
    string string_retour=retour;
    delete retour;
    return string_retour;
}

void SocketObjet::connection(){
    if(connect(socket_, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
        perror("CONNECT : ");
        exit(1);
    }
}

void SocketObjet::binder(){//pour un serveur
    if(bind(socket_, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("BIND : ");
        exit(1);
    }
}

void SocketObjet::ecouter(){
    if(listen(socket_, backlog) == -1){
        perror("LISTEN : ");
        exit(1);
    }
}

int SocketObjet::accepter(){
    sin_size = sizeof(struct sockaddr_in);
    mtx.lock();
    for(int i=0; i<socketfd_.size();++i){
        if(socketfd_[i]==-1){
            socketfd_[i]=accept(socket_, (struct sockaddr *)&their_addr, (socklen_t*)&sin_size);
            int sortie = i;
            if( socketfd_[i] == -1){
                perror("ACCEPT : ");
                exit(1);
            }
            mtx.unlock();
            printf("server: (recyclage) got connection from %s\n", inet_ntoa(their_addr.sin_addr));
            return sortie;
        }
    }
    socketfd_.push_back(accept(socket_, (struct sockaddr *)&their_addr, (socklen_t*)&sin_size));
    int sortie = socketfd_.size()-1;
    if( socketfd_[socketfd_.size()-1] == -1){
        perror("ACCEPT : ");
        exit(1);
    }
    mtx.unlock();
    printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    return sortie;
}

void SocketObjet::fermer(int sockfd){
    if(sockfd != -1){
        if(close(socketfd_[sockfd]) == -1){
            perror("CLOSE : ");
            exit(1);
        }
        socketfd_[sockfd]=-1; 
        printf("server: got deconnexion\n");
        return;
    }
    else{
        if(close(socket_) == -1){
            perror("CLOSE : ");
            exit(1);
        }

        printf("server: got deconnexion\n");
    }
}

 int SocketObjet::getMax(){
     int max=0;
     for(int i=0;i<socketfd_.size();i++){
        if(max<socketfd_[i]){
            max=socketfd_[i];
        }
     }
     return max;
 }

 void SocketObjet::envoyerGroup(char * c, vector<int> destinataires){
    for (int i=0; i<destinataires.size(); ++i){
        if(send(destinataires[i], c, strlen(c),0)==-1){
                perror("SEND : ");
                exit(1);
        }
    }
 }