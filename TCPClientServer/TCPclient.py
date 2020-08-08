#!/usr/bin/python3

import tkinter as tk
from tkinter import messagebox
from io import StringIO
import socket
import sys
import select 
import pdb
import time

# Constants
PORT = 8080 # Socket will open on Port 8080
MAXLEN = 80 # Maximum amount of characters buffer will need

# Global Variables
the_sock = 0

# Get message from text box
def getMsg(user_entry, client_sock):
   msg = user_entry.replace('\n', '')
   client_sock.send(msg.encode('utf8'))
   tkMessage.delete(1.0, tk.END)
   
# Facilitate activity on the client
def chat(sock, window):
    while True:
       
        try: 
            window.update_idletasks()
            window.update()
        except:
            print("Client has been terminated")
            sys.exit(0)

        if sock == 0:
            time.sleep(1)
            continue

        # Begin listening for activity on file descriptors 
        fileDescriptors, _, _ = select.select([sock], [], [], 0.2)
        for fd in fileDescriptors:

            # If the client is picking something up from the server
            if fd is sock:
                tkDisplay.config(state=tk.NORMAL)
                texts = tkDisplay.get("1.0", tk.END).strip()
                temp = sock.recv(1024)
                recieved = temp.decode("utf-8")
                if recieved == "":
                    sock.close()
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
                   
# Set up client socket
def connect():
   
    # Initialize the socket 
    global the_sock
    the_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if the_sock == -1:
        sys.exit("Error: Socket creation failed")
    else:
        print("Socket successfully created")

    # Connect to the server address
    server_address = ('localhost', PORT)
    print("Connecting to %s Port %s" % server_address)
    the_sock.connect(server_address)
    print("Socket has been connected")


# Chat window for client
window = tk.Tk()
window.title("Client")

topFrame = tk.Frame(window)
btnConnect = tk.Button(topFrame, text="Connect", command=lambda : connect())
btnConnect.pack()
topFrame.pack(side=tk.TOP)

displayFrame = tk.Frame(window)
lblLine = tk.Label(displayFrame, 
text="*************************** Chat Server ***************************").pack()

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
bottomFrame.pack(side=tk.BOTTOM)

while the_sock == 0:
    try:
        window.update_idletasks()
        window.update()
        time.sleep(0.5)
    except:
        print("Client has been terminated")
        sys.exit(0)

tkMessage.bind("<Return>", (lambda event: getMsg(tkMessage.get("1.0", tk.END), the_sock)))
tkMessage.config(state=tk.NORMAL)

chat(the_sock, window)

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

