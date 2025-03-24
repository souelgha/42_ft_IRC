# FT_IRC Project
this is a project group and we have to ceate our own IRC server in C++98 from scratch. 

## Purpose
- We used an actual IRC client (IRSSI or netcat) to connect to our server and test it.
- The server follows the [RFC2812 specifications ](https://www.rfc-editor.org/rfc/rfc2812) can therefore be used with existing IRC clients.
- **IRC** (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in **1988**.
- Our server must be capable of handling multiple clients at the same time and never hang.

## Build ans start:
Run a make
``` bash
make
```
To start server use 
``` bash
./ircserver <port> <password>
```
- port: The port number on which your IRC server will be listening to for incoming IRC connections.
- password: The connection password.

To connect to the server use:
``` bash
nc <IP ADDRESS> <PORT>
```
- IP ADDRESS: Host IP address.
- PORT: The PORT that the server listening on.
You can also use an IRC Client IRSSI.

## chat example in channel using IRSSI client
![apercu](/irc-chat.png)



