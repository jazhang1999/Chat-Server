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

# Changelog
Since I find commit messages on this repo to not be able to accuratly describe all changes going on, I will be putting all relevent edits and revisions here instead

# Personal Notes
These are just some things that I learned. Since this project was as much an exercise on coding and development as well as making a working application, I will list some interesting tidbits I picked up as a result of this experience

* __Python better script running__ - previously, I used to run the Python client code by doing `python3 TCPclient.py`. Now, I use a shebang `#!/usr/bin/python3` at the top of the file, and then ran `chmod 755 TCPclient.py` to be able to run the file by doing `./TCPclient.py`



