# Network Configuration

The PC and W5500 are connected directly through Ethernet.

## Static addressing

| Device | IPv4 |
|---|---|
| Windows PC | 169.254.228.250 |
| STM32/W5500 | 169.254.228.251 |

Subnet mask:

`255.255.0.0`

Gateway:

`0.0.0.0`

TCP server port:

`5000`

W5500 MAC:

`02:08:DC:34:56:78`

## Verification

### Ping

```powershell
ping 169.254.228.251
```

The tested setup returned 4/4 replies with 0% packet loss.

### TCP port

```powershell
Test-NetConnection 169.254.228.251 -Port 5000
```

The final test returned:

```
TcpTestSucceeded : True
```
