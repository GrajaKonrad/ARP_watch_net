# Program ARP watch net
## Program function
The "ARP Watch Net" program is a network monitoring tool designed to observe and log ARP (Address Resolution Protocol) traffic in a network. Its main goal is to monitor any discrepancies found, between hosts, in received ARP packets, and then warn user of security threat that has occured. This tool is created in mind for network diagnostics, security audits, and understanding network traffic patterns.

- ARP Watch Net client - captures ARP packets transmitted over the network, extracts relevant information such as sender's IP and MAC addresses, logs data locally and sends information, over UDP, about any new or changed pakcets, to server for further analysis. Using alternate mode it is posible to monitor system ARP file containing ARP information.
- ARP Watch Net server - collects logs sent by clients, analyzes it, and warns local user of any danger that might occure on monitored hosts. 


## Requirements
- C++ 17 or higher
- MakeFile
- sudo user for program client operation

## Compilation
To compile program simply type #make while in project main directory.\

Project source files:
AppStart - entry point to the program, it sets up all the required by user switches, and launches selected instance of the program.\
Client - client code, including both operational modes, capture tool, and logger.\
Server -  server, recives information from clients, analyzes it, and eventually warns user.\
StartupOptions - contains code required to set default values in the program.

## Program Execution
Program can be executed in two modes: server and client as per described in program function section.
Note that the program client requires sudo user to work properly. 

To start the program in server mode use:\
```
./bin/ArpWatchNet -s [server port]

example:
```
./bin/ArpWatchNet -s 12345
```

To start the program in client mode with packet capture use:\
```
sudo ./bin/ArpWatchNet -c [connect to server IP] [connect to server port] -p [interface that requires ARP montitoring]
```

example:
```
./bin/ArpWatchNet -s 127.0.0.1 12345 -p eth0
```


To start the program in client mode with ARP file watch use:\
```
sudo ./bin/ArpWatchNet -c [connect to server IP] [connect to server port] -f
```

example:
```
sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -f
```

## Program Switches
-c [IP] [PORT] - Start program in client mode. You need to specify the IP and port of the server that collects client logs\
-s [PORT] - Start program in server mode. You need to sepcify on what port server listens for incoming logs from clients\
-v - Verbose output\
-f - CLIENT Only. Start in file montitor mode. Uses ARP file to track changes in ARP table, insted of captured packets\
-p [INTERFACE] - CLIENT Only. Start in packet capture mode. Uses captured ARP packets to identify new and changed ARP entries\
-a [ARP_PATH] - CLIENT Only, only viable with -f: Specified custom path to ARP file, default "/proc/net/arp" \
-t [ARP_INTERVAL] - CLIENT Only, only viable with -f: Specified custom time between subsequent file reads in secounds, default 10 sec\
