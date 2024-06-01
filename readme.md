# Prgram ARP watch net
## Program function
The "ARP Watch Net" program is a network monitoring tool designed to observe and log ARP (Address Resolution Protocol) traffic in a network. It captures ARP packets transmitted over the network, extracts relevant information such as sender's IP and MAC addresses, logs data and sends it to simple server for analysis. Using alternate mode it is posible to monitor system ARP file conteining ARP information. This can be useful for network diagnostics, security audits, and understanding network traffic patterns.

## Compilation
To compile program simply type make. Note that the program requires Cpp 17 or higher to be compiled.

AppStart - contains all program switches and is an entry point to a program.
Client - contains all client code, including both operational modes.
Server - contains simple server to recive and print info packets.
StartupOptions - contains simple code to set some default values in the program.

## Program Execution

To start the program in server mode use:
make run_server
or
./bin/ArpWatchNet -s 12345

To start the program in client mode with packet capture use:
make run_client_port
or
sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -p eth0

To start the program in client mode with ARP file watch use:
make run_client_file
or
sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -f


## Program Switches
-c [IP] [PORT] - Client mode"
-s [PORT] - Server mode"
-f - Use ARP file to track changes in ARP table"
-p [INTERFACE] - Use Packet Capture to identify new ARP entries"
-v - Verbose output"
-a [ARP_PATH] - CLIENT Only: Specified own ARP Path"
-t [ARP_INTERVAL] - CLIENT Only: Specified own ARP watch arp-file-read interval"
