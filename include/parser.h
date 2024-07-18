#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



struct httpRequest{
    char *method;
    char *path;
};

bool pathExpected = false;


char *METHODS[] = {
    "GET",
    "POST",
};

char *SEPARATORS[]={
    "/"
};

int appendChar(char *buffer,char C){
    int len = strlen(buffer);
    buffer[len]   = C;
    buffer[len+1] = '\0';
}



void parseRequest(char *buffer,struct httpRequest *request){
    bool gotPath = false;
    int index = 0;
    int fileEND = strlen(buffer);
    char currentToken[50] = "";

    while (index != fileEND){
        char C = buffer[index];
        if (C != ' '){
        appendChar(currentToken,C);
        }
        index++;
        if (C == '/' && gotPath == true){
            while(C != ' '){
                C = buffer[index];
                appendChar(currentToken,C);
                index ++;
            }
            request->path = malloc(strlen(currentToken));
            strcpy(request->path,currentToken);      
            break;  
        }
        if (gotPath == false){
        for (int i = 0; i < sizeof(METHODS)/8; i++){
            if (strcmp(currentToken,METHODS[i]) == 0){
                for (int j = 0; j < strlen(currentToken);j++){
                    request->method = malloc(strlen(currentToken));
                    strcpy(request->method,currentToken);
                    memset(currentToken,0,strlen(currentToken));
                    gotPath = true;
                }
            }
        }
        continue;
     }
 }
}







