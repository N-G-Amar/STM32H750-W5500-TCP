# Hardware Setup

## Components

- WeAct Studio STM32H750VBT6 board
- W5500 Ethernet module
- ST-Link debugger/programmer
- Ethernet cable

## SPI wiring

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

The W5500 communicates with the STM32 through SPI1. The external W5500 Ethernet controller is used instead of the STM32's internal Ethernet MAC.

## Debug connection

The ST-Link is connected to the dedicated SWD header:

- 3.3V
- SWDIO
- SWCLK
- GND

The WeAct USB-C connection can provide board power.
