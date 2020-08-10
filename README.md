# Overview
This project attempts to create a chat application, that functions like a primitive form of other messaging softwares (WeChat, Apple msg, etc.). Personally, I used it to practice writing in different languages as well as experimenting with socket programing. 

(Insert Picture 1 Here) 

In terms of which protocol was used to send data, I used TCP since it seemed the most straightforward to implement. That is why all relevent code is written under the `/TCPClientServer` folder. 

There are 2 components to this project:
* Server - This section is written in C, and assists in verifying and welcoming clients to the service. It also faccilitates the transfer of messages back and forth between multiple clients. The code for the server is located within the file `TCPserver.c`
* Client - This section is written in Python, and provides a GUI from which the user can communicate to other users. The code for the client is located within the file `TCPclient.py`
We will discuss these applications in greater detail below

Additional Notes:
* The Makefile as of right now only compiles the TCPserver.c code into a runnable executable, `server.x` that is used to launch the server. 
* There is a directory `/Experimental` that contains code not used in the project, but is being archived as it may be useful at a later date
* The file `C-TestingGrounds.c` is an area where I used to test certain C concepts. C is not my strongest language so I always like to have an area to test out what I learn from online as well as review what I learned from lectures

# Server (TCPserver.c)
To create the executable for the server, I would run `make clean` to remove any old object files and executables, then run `make` to build. The executable produced should be name `server.x`. Running `./server.x` will launch the server.

The server code does these things:
* Sets up the server by initializing the "master socket" - the socket that is associated with the server. I have also configured the server to pick up to a max of 5 different connections. The process is done in function `set_up_server`
* Now the server will begin functioning until the executable is forcibly exited (Ctrl-C). We used the `select` function to handle multiple incoming communiques between different clients or from client to server. We found this prefereable to using multiple threads to handling different processes concurrently, since that would require an extra layer of complexity. 
* The server also provides a means of verification. When a user (i.e client) is detected trying to connect to the server, the server will initially prompt the user to enter in a username and password. Should these credentials be correct, the user will be added to the list of accepted client sockets (allowed to participate). Otherwise, the server will reject the incoming connection, and the client side will be shut down. __(The verification process will likely be overhauled later on - this is still in the preliminary test stages)__

This is the basic overview of the function of the server code. More detailed comments are listed at the source code if there are any lingering questions

# Changelog
Since I find commit messages on this repo to not be able to accuratly describe all changes going on, from now on I will be putting all relevent edits and revisions here instead

# Personal Notes
These are just some things that I learned. Since this project was as much an exercise on coding and development as well as making a working application, I will list some interesting tidbits I picked up as a result of this experience

* __Python better script running__ - previously, I used to run the Python client code by doing `python3 TCPclient.py`. Now, I use a shebang `#!/usr/bin/python3` at the top of the file, and then ran `chmod 755 TCPclient.py` to be able to run the file by doing `./TCPclient.py`

# Future Changes
* __Ability to Modify/Add-in users__ - Currently, I have predefined 5 username/password pairs that can be used to sign into the chat application. This was done for testing, but in the future I hope to create a system for which new users can be added, and these credentials are not stored in `TCPserver.c`, where the sever code is located.



