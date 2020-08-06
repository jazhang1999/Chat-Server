#!/usr/bin/python3

import tkinter as tk
from tkinter import messagebox
import socket
import sys
import select 
import pdb

# Constants
PORT = 8080 # Socket will open on Port 8080
MAXLEN = 80 # Maximum amount of characters buffer will need

# Facilitate activity on the client
def chat(client_sock):
    while True:

        # Begin listening for activity on file descriptors 
        fileDescriptors, _, _ = select.select([sys.stdin, client_sock], [], [])
        for fd in fileDescriptors:

            # If the client is picking something up from the server
            if fd is client_sock:
                tkDisplay.config(state=tk.NORMAL)
                texts = tkDisplay.get("1.0", tk.END).strip()
                temp = client_sock.recv(1024)
                recieved = temp.decode("utf-8")
                """
                for character in from_buffer:
                    if 31 < ord(character) < 126:
                        recieved += character
                """
                print(len(recieved)) 
                if recieved == "":
                    client_sock.close()
                    window.destroy()
                    sys.exit(0)
                else:
                    if len(texts) < 1:
                        tkDisplay.insert("1.0", recieved)
                    else:
                        tkDisplay.insert(tk.END, '\n' + recieved)
                
                tkDisplay.config(state=tk.DISABLED)
                tkDisplay.see(tk.END)
                tkDisplay.update()
                print("Server says: " + recieved)
                    
            # If there is input detected from the client
            else:
                msg = sys.stdin.readline()
                client_sock.send(msg.encode("utf8"))
        
# Set up client socket
def connect():

    # Create the socket 
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if client_sock == -1:
        sys.exit("Error: Socket creation failed")
    else:
        print("Socket successfully created")
    
    # Configure the Chat box 
    # entName.config(state=tk.DISABLED)
    # btnConnect.config(state=tk.DISABLED)
    # tkMessage.config(state=tk.NORMAL)  

    # Connect to the server address
    server_address = ('localhost', PORT)
    print("Connecting to %s Port %s" % server_address)
    client_sock.connect(server_address)
    chat(client_sock)




# Chat server for client
window = tk.Tk()
window.title("Client")
#username = " "

tkDisplay = tk.Text(height=20, width=55)
tkDisplay.pack()

btnConnect = tk.Button(text="Connect", command=lambda : connect())
btnConnect.pack(side=tk.LEFT)


"""
topFrame = tk.Frame(window)
lblName = tk.Label(topFrame, text = "Name:").pack(side=tk.LEFT)
entName = tk.Entry(topFrame)
entName.pack(side=tk.LEFT)
btnConnect = tk.Button(topFrame, text="Connect", command=lambda : connect())
btnConnect.pack(side=tk.LEFT)
#btnConnect.bind('<Button-1>', connect)
topFrame.pack(side=tk.TOP)

displayFrame = tk.Frame(window)
lblLine = tk.Label(displayFrame, 
text="*********************************************************************").pack()

scrollBar = tk.Scrollbar(displayFrame)
scrollBar.pack(side=tk.RIGHT, fill=tk.Y)

tkDisplay = tk.Text(displayFrame, height=20, width=55)
tkDisplay.pack(side=tk.LEFT, fill=tk.Y, padx=(5, 0))
tkDisplay.tag_config("tag_your_message", foreground="blue")
scrollBar.config(command=tkDisplay.yview)
tkDisplay.config(yscrollcommand=scrollBar.set, background="#F4F6F7", 
highlightbackground="grey", state="disabled")
displayFrame.pack(side=tk.TOP)

bottomFrame = tk.Frame(window)
tkMessage = tk.Text(bottomFrame, height=2, width=55)
tkMessage.pack(side=tk.LEFT, padx=(5, 13), pady=(5, 10))
tkMessage.config(highlightbackground="grey", state="disabled")
tkMessage.bind("<Return>", (lambda event: getChatMessage(tkMessage.get("1.0", tk.END))))
bottomFrame.pack(side=tk.BOTTOM)
"""
tkDisplay.insert("1.0", "Hello")
window.mainloop() 

