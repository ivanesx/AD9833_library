# AD9833_library
control library ad9833 with stm32
tested on stm32h7 in STM32CubeIDE environment
The NSS pin will be pulled up to the power supply, through a 10 kΩ resistor

SYSCLK 480MHz

Setting SPI 
mode transmit only master
hardware nss signal hardware NSS output signal
data size 16 bit
msb first
prescaler 128
CPOL High
CPHA 2 Edge
NSS sygnal type output hardware
NSS polarity NSS polarity low

connection
STM32         AD9833
SPI_MOSI ->   SDATA
SPI_SCK  ->   SCLK
SPI_NSS  ->   FSYNC

I took the synchronization for the oscillator from an external 10 MHz oscillator, the output was controlled by an oscilloscope.
