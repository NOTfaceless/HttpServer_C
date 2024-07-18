#include "std.h"



struct httpServer{
    char *port;
    char *ipAdress;
    int sockType;
    int family;
    int backlog;
    int socket;
    bool shutDown;
};

int startServer(struct httpServer *server){
    int serverScoket;
    struct addrinfo hints,*service;

    memset(&hints,0,sizeof hints);
    hints.ai_family  = server->family;
    hints.ai_socktype = server->sockType;

    if (getaddrinfo(server->ipAdress,server->port,&hints,&service) != 0){
        printf("getaddrinfo failed\n");
    }

    serverScoket = socket(service->ai_family,service->ai_socktype,service->ai_protocol);
    if (serverScoket == SOCKET_ERROR){
      printf("Socket creation failed");
      return SOCKET_ERROR;
    }

    if (bind(serverScoket,service->ai_addr,service->ai_addrlen) == SOCKET_ERROR){
        printf("Failed to bind socket to port:%s\n",server->port);
        printf("Port might be unavilble\n");
        return SOCKET_ERROR;
    }
   
    if (listen(serverScoket,server->backlog) == SOCKET_ERROR){
       printf("socket listening failed\n");
       return SOCKET_ERROR;    
    }
 
    server->socket = serverScoket;
    server->shutDown = false;

    printf("SERVER STARTED LISTENING\n");
    printf("IP ADRESS:%s\n",server->ipAdress);
    printf("PORT NUMBER:%s\n",server->port);

    return 0;
}


