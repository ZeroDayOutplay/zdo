#include <stdio.h>
#include <winsock2.h>
#include <string.h>


int main(int argc, char **argv){

    WSADATA wsa; //WSADATA structure contains information about the Windows Sockets implementation.
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) //Initialising Winsock API
	{
		return 1;
	}

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock){
        return 1; // return Error when sock obj has failed
    }

    struct sockaddr_in add;

    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr("10.0.2.15");
    add.sin_port = htons(1234);

    connect(sock, (struct sockaddr *) &add, sizeof(add));

    char msg[1024];
    recv(sock, msg, sizeof(msg), 0);
    printf("%s", msg);

    return 0;
}