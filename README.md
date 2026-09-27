# STM32H750 + W5500 TCP Ethernet

A working TCP/IP communication project using a **WeAct Studio STM32H750VBT6** board and an external **W5500 Ethernet controller** over SPI.

The project was tested end-to-end from a Windows PC using a Python TCP client.

## Hardware

- WeAct Studio STM32H750VBT6
- W5500 Ethernet module with HanRun RJ45
- ST-Link programmer/debugger
- Windows PC with Ethernet connection

## W5500 wiring

| W5500 | STM32H750 |
|---|---|
| SCLK | PA5 / SPI1_SCK |
| MISO | PA6 / SPI1_MISO |
| MOSI | PA7 / SPI1_MOSI |
| SCS / CS | PA4 |
| RST | PC0 |
| GND | GND |
| 3.3V | 3.3V |
| 5V | Not connected |
| INT | Not connected |

## STM32 configuration

- MCU: STM32H750VBT6
- External HSE: 25 MHz
- SYSCLK: 400 MHz
- SPI1: full-duplex master
- SPI mode: Motorola, CPOL low, CPHA 1st edge
- SPI data size: 8-bit
- SPI prescaler: /16
- SPI clock: 12.5 MHz
- Hardware Ethernet peripheral: disabled
- W5500 is controlled through SPI

## Network configuration

The project uses a direct Ethernet connection with static IPv4 addresses.

| Device | Address |
|---|---|
| Windows PC | 169.254.228.250 |
| W5500 / STM32 | 169.254.228.251 |
| Subnet mask | 255.255.0.0 |
| Gateway | 0.0.0.0 |
| TCP port | 5000 |

W5500 MAC address:

`02:08:DC:34:56:78`

## TCP server

Socket 0 is configured as a TCP server on port **5000**.

The firmware:

1. Resets the W5500.
2. Reads VERSIONR.
3. Configures the W5500 network registers.
4. Opens Socket 0 in TCP mode.
5. Listens on port 5000.
6. Reads received bytes from the RX buffer.
7. Advances the RX read pointer and issues the RECV command.
8. Writes the received data to the TX buffer and issues SEND.
9. Reopens the socket after a disconnect.

The current implementation uses 2 KB Socket 0 RX/TX buffers and a 256-byte STM32-side receive buffer for the simple echo test.

## Verified tests

### 1. W5500 detection

W5500 VERSIONR returned:

`0x04`

### 2. Network connectivity

Windows ping test:

`ping 169.254.228.251`

Result:

- 4 packets sent
- 4 packets received
- 0% packet loss

### 3. TCP port test

Windows:

`Test-NetConnection 169.254.228.251 -Port 5000`

Result:

`TcpTestSucceeded : True`

### 4. End-to-end TCP echo

Python client connected to the STM32/W5500 server and exchanged data successfully:

```
Connecting to STM32...
Server: 169.254.228.251:5000
Connected to STM32 W5500!
Sending: HELLO STM32
Received: HELLO STM32
Connection closed.
```

This verifies bidirectional application-level TCP communication between the PC and STM32 through the W5500.

## Python client

The test client is in:

`python/tcp_client.py`

The current command used on Windows is:

```powershell
uv run python tcp_client.py
```

## Project structure

```
STM32H750-W5500-TCP/
├── README.md
├── firmware/
│   └── W5500_STM32H750/
│       └── Core/
│           └── Src/
│               └── main.c
├── python/
│   └── tcp_client.py
├── docs/
│   ├── hardware.md
│   ├── network.md
│   └── testing.md
└── hardware/
    └── wiring.md
```

## Build result

STM32CubeIDE build:

- 0 errors
- 0 warnings
- ELF generated successfully

## Notes

This repository documents the tested TCP echo milestone. More application-level functionality can be added on top of the working TCP transport, such as commands, sensor telemetry, actuator control, or a custom PC application.
