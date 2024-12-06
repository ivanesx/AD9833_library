/***************************************************************************//**
 * File    AD9833.h
 * Title   AD9833 Wave Generator for STM32 HAL
 * Author  Ivan Vladimirovich Kozyrev <ivanesxxx@gmail.com>
 * Created on Nov 19, 2021
 * Software STM32CubeIDE
 * Hardware All STM32 with hardware SPI
*******************************************************************************/
#ifndef AD9833_H_
#define AD9833_H_

#include <math.h>
#include "stm32h7xx_hal.h"

// ------------------------- Defines -------------------------
#define FMCLK                 1e7                // Master Clock On AD9833
#define FREQUENCYFACTOR      0x10000000         // 2^28

// Waveform control bits
#define SINWAVE              0x0000
#define SQUAREWAVE           0x0028
#define TRIANGLEWAVE         0x0002
#define RESETBIT             0x0100
#define CONTROLBITTWOFREQUENCY 0x2000

// Frequency and Phase Control Bits
#define FREQZEROREGBITS      0x4000
#define PHASEZEROREGBITS     0xc000

// Wave Type Enum
typedef enum {
    SIN = 0,
    SQR,
    TRI
} WaveType;

// External SPI interface
extern SPI_HandleTypeDef hspi1; // Used SPI interface

// ------------------ Function Prototypes ---------------------
void AD9833_SetWave(WaveType wave);         // Set wave type: sine, triangular, square
void AD9833_SetPhase(uint16_t phase);       // Phase shift in degrees
void AD9833_SetFrequency(uint32_t frequency); // Set frequency
void AD9833_Reset(void);                     // Reset

#endif /* AD9833_H_ */
