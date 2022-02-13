#include <stdio.h>
#include <winsock2.h>
#include <string.h>


int main(int argc, char **argv){

    WSADATA wsa; //WSADATA structure contains information about the Windows Sockets implementation.
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) //Initialising Winsock API
	{
        printf("WINSOCK ERROR!");
		return 1;
	}

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET){
        printf("SOCKET ERROR!");
        return 1; // return Error when sock obj has failed
    }

    struct sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr("10.0.2.15");
    add.sin_port = htons(1234);

    if (bind(sock, (struct sockaddr *) &add, sizeof(add)) == SOCKET_ERROR){
        printf("BIND ERROR!");
        return 1; // bind the server
    }

    listen(sock, 3);

    printf("[+] Listening for Incomming Connections...\n");

    int c = sizeof(struct sockaddr_in);
    int client_sock = accept(sock, (struct sockaddr *) &add, &c);
    if (client_sock == INVALID_SOCKET){
        printf("ACCEPTING ERROR!");
        return 1;
    }

    printf("[+] Target Connected.\n");

    char *msg;
    msg = "Hey Meda\n";

    send(client_sock, msg, strlen(msg), 0);

    closesocket(sock);
    WSACleanup();

    return 0;
}