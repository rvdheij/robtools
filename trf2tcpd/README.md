# TRF2TCPD - Convert VM TCP/IP Traces to pcap

## Packet Trace in TRF files
Create the TRF files as described in *TCP/IP Diagnostics*
Chapter 7 section *TCP/IP Stack Packet Trace with TRSOURCE*.
Cheat sheet below (replace ETH0 with the device name (from
(from ```NETSTAT DEVLINKS```). You may also want to specify
full packet capture rather than just 64 byte because you'll
be using tools to decode the records.
```
netstat obey packettracesize 9000
netstat obey traceonly eth0 endtraceonly
trsource id testcase type gt block for user TCPIP
trsource ena id testcase
```
After traffic has been captured, close the trace with
```
trsource disa id testcase
```
and find the TRF file number with ```CP Q TRF``` so you can
process the data with
```
TRF2TCPD *trf* *fname ftype fm*
```
Remember to use binary transfer to get the data to your
workstation where **Wireshark** and similar tools are your
friend.

## Converting TRACERED Output
The TRF file can be processed by TRACERED (for example
to transport it to another system) like this:
```
TRACERED *trf* CMS *fname* TRACERED A
```
You can convert this file with
```
PIPE < *fname* TRACERED A | trc2tcpd | > *fname ftype fm*
```
