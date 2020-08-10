# Overview
This project attempts to create a chat application, that functions like a primitive form of other messaging softwares (WeChat, Apple msg, etc.). Personally, I used it to practice writing in different languages as well as experimenting with socket programing. 

In terms of which protocol was used to send data, I used TCP since it seemed the most straightforward to implement. That is why all relevent code is written under the `/TCPClientServer` folder. 

There are 2 components to this project
* Server - This section is written in C, and assists in verifying and welcoming clients to the service. It also faccilitates the transfer of messages back and forth between multiple clients
* Client - This section is written in Python, and provides a GUI from which the user can communicate to other users
