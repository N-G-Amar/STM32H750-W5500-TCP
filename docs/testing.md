# Testing

## End-to-end TCP echo

The project was tested from Windows using a Python TCP client.

Run from the Python client directory:

```powershell
uv run python tcp_client.py
```

Expected output:

```
Connecting to STM32...
Server: 169.254.228.251:5000
Connected to STM32 W5500!
Sending: HELLO STM32
Received: HELLO STM32
Connection closed.
```

The returned message confirms that data was received by the STM32/W5500 TCP server and sent back to the PC.

## Milestone

At this stage the project has verified:

- SPI communication between STM32H750 and W5500
- W5500 identification
- W5500 network configuration
- Ethernet link
- IP connectivity
- TCP listening on port 5000
- TCP connection from Windows
- TCP receive
- TCP transmit
- Application-level echo

## Development tools

- STM32CubeMX 6.18.1
- STM32CubeIDE 2.2.0
- STM32CubeH7 firmware package V1.13.0
- Windows PowerShell
- Python via uv
