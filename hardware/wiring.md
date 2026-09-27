# W5500 Wiring Reference

```
W5500              STM32H750VBT6
--------------------------------
SCLK   ----------> PA5  (SPI1_SCK)
MISO   <---------- PA6  (SPI1_MISO)
MOSI   ----------> PA7  (SPI1_MOSI)
SCS    ----------> PA4  (CS)
RST    ----------> PC0  (RESET)
GND    ----------> GND
3.3V   ----------> 3.3V
5V     ----------> NOT CONNECTED
INT    ----------> NOT CONNECTED
```

SPI configuration:

- Master
- Full duplex
- 8-bit
- MSB first
- CPOL = Low
- CPHA = 1st Edge
- Prescaler = /16
- SPI clock = 12.5 MHz
