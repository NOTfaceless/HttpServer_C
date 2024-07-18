#include "include/server.h"
#include "include/parser.h"
#include <errno.h>


int checkExtension(char** ContentType,char* fullPath){
    int pathLen = strlen(fullPath);
    for (int i = 0; i <= pathLen; i++){
        if (fullPath[i] == '.'){
            if (fullPath[i+1] == 'p'){
                *ContentType ="application/octet-stream";       
            }else if(fullPath[i+1] == 'j'){
                 *ContentType =" application/javascript";
            }else if (fullPath[i+1] == 'h'){
                *ContentType = "text/html";
            }else if (fullPath[i+1] == 'c'){
              *ContentType ="text/css";
            }
        }
    }
    return 0;
}

int getPage(int socket,char *filePath){
    size_t len = strlen(filePath);
    filePath[len - 1] = '\0';
    printf("%i",len);
    
    if (strlen(filePath) > 49 ){
        printf("no buffer overflow\n");
        return -1;
    }

    char fullPath[50] = "website/home";

    if (filePath[0] == '/' && len < 3){
        printf("Ran\n");
        strcat(fullPath,"/index.html");
    }else{
        strcat(fullPath,filePath);
    }
   
    char header[200];
    char *body;
    char *response;
    char rep2[512];

    printf("GET REQUEST FOR:%s\n",fullPath);

    FILE *file;
    file = fopen(fullPath,"rb");
    
    if (file == NULL && errno == 2){

        printf("PATH DOSEN'T EXIST:%s\n",fullPath);
        return -1;
    }
    
    char *ContentType;
    checkExtension(&ContentType,fullPath);


    size_t  ContentLength;
    fseek(file,0,SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    body = malloc(fileSize);

    size_t bytesRead = fread(body,1,fileSize,file);

    ContentLength = fileSize;

    
    if (bytesRead == 0){
        printf("ERROR READING IMG FILE\n");
    }     
    
    snprintf(header,sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %d\r\n"
             "Content-Type: %s\r\n"
             "\r\n", ContentLength, ContentType);

    send(socket,header,strlen(header),0);
    send(socket,body,ContentLength,0);
    free(body);

    if (fclose(file) == 0){
        printf("FILE HAS BEEN CLOSED\n");
    }else{
        printf("failed to clsoe file\n");
    }
    return 0;
}


int handleRequest(struct httpRequest *request,int socket){
    printf("METHOD:%s\nPATH:%s\n",request->method,request->path);
    if (strcmp(request->method,"GET") == 0){
        getPage(socket,request->path);
    }
    free(request->method);
    free(request->path);
    free(request);
    return 0;
}

int main(void){
    struct httpServer server;
    server.port     = "2003";
    server.ipAdress = "127.0.0.1";
    server.sockType = SOCK_STREAM;
    server.family   = AF_UNSPEC;
    server.backlog  = 1;

    int started = startServer(&server);
    if (started == 0){
    
    while (!server.shutDown){


    struct sockaddr_storage socketAdress;
    socklen_t addrSize = sizeof socketAdress;

    int newScoket = accept(server.socket,(struct sockaddr*)&socketAdress,&addrSize);
    if (newScoket == SOCKET_ERROR){
        printf("connection failed\n");
        return SOCKET_ERROR;
    }

    char dataRecived[512];
    int bytes = recv(newScoket,dataRecived,sizeof(dataRecived),0);
    
    if (bytes <=0){
        close(newScoket);
    }
    
    struct httpRequest *request;
    request = malloc(sizeof(struct httpRequest));
    parseRequest(dataRecived,request);
    handleRequest(request,newScoket);
    
    close(newScoket);
    }
  }

    return 0;
}

