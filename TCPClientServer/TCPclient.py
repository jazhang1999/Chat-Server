#!/usr/bin/python3

import socket
import sys
import select 

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
                print(client_sock.recv(1024).decode("utf8"))

            # If there is input detected from the client
            else:
                msg = sys.stdin.readline()
                client_sock.send(msg.encode("utf8"))
        
# Set up client socket
def main():
    # Create the socket 
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if client_sock == -1:
        sys.exit("Error: Socket creation failed")
    else:
        print("Socket successfully created")
    
    # Connect to the server address
    server_address = ("localhost", PORT)
    print("Connecting to %s Port %s" % server_address)
    client_sock.connect(server_address)
    print("Connected")   
    chat(client_sock)

# Runner
if __name__ == "__main__":
    main()

    
    
        
    
