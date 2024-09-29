
# ARP Watch Net

## Program Function
The **ARP Watch Net** program is a network monitoring tool designed to observe and log ARP (Address Resolution Protocol) traffic in a network. Its main goal is to monitor discrepancies between hosts in received ARP packets and warn the user of security threats that have occurred. This tool is designed with network diagnostics, security audits, and understanding network traffic patterns in mind.

### Components:
- **ARP Watch Net Client**  
  Captures ARP packets transmitted over the network, extracts relevant information such as the sender's IP and MAC addresses, logs data locally, and sends information over UDP about any new or changed packets to the server for further analysis. In an alternate mode, it can monitor the system ARP file containing ARP information.

- **ARP Watch Net Server**  
  Collects logs sent by clients, analyzes them, and warns the local user of any dangers that might occur on monitored hosts.

## Requirements
- C++ 17 or higher
- Makefile
- `sudo` privileges for client operation

## Compilation
To compile the program, simply type the following command while in the project's main directory:

```bash
make
```

### Project Source Files:
- **AppStart**: The entry point to the program, sets up all required user switches, and launches the selected instance of the program.
- **Client**: Client code, including both operational modes, the capture tool, and the logger.
- **Server**: Receives information from clients, analyzes it, and eventually warns the user.
- **StartupOptions**: Contains code required to set default values in the program.

## Program Execution
The program can be executed in two modes: server and client, as described in the program function section.  
Note that the client requires `sudo` privileges to work properly.

### Server Mode
To start the program in server mode, use the following command:

```bash
./bin/ArpWatchNet -s [server port]
```

**Example**:
```bash
./bin/ArpWatchNet -s 12345
```

### Client Mode (Packet Capture)
To start the program in client mode with packet capture, use:

```bash
sudo ./bin/ArpWatchNet -c [server IP] [server port] -p [interface that requires ARP monitoring]
```

**Example**:
```bash
sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -p eth0
```

### Client Mode (ARP File Watch)
To start the program in client mode with ARP file watch, use:

```bash
sudo ./bin/ArpWatchNet -c [server IP] [server port] -f
```

**Example**:
```bash
sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -f
```

## Program Switches

- `-c [IP] [PORT]`: Start the program in client mode. You need to specify the IP and port of the server that collects client logs.
- `-s [PORT]`: Start the program in server mode. You need to specify on which port the server listens for incoming logs from clients.
- `-v`: Verbose output.
- `-f`: **CLIENT Only**. Start in file monitor mode. Uses the ARP file to track changes in the ARP table, instead of captured packets.
- `-p [INTERFACE]`: **CLIENT Only**. Start in packet capture mode. Uses captured ARP packets to identify new and changed ARP entries.
- `-a [ARP_PATH]`: **CLIENT Only, only viable with -f**: Specifies a custom path to the ARP file, default "/proc/net/arp".
- `-t [ARP_INTERVAL]`: **CLIENT Only, only viable with -f**: Specifies the custom time between subsequent file reads in seconds, default 10 seconds.
