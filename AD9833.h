/***************************************************************************//**
 * File	AD9833.h
 * Title	AD9833 Wave Generator for STM32 HAL
 * Author:    Ivan Vladimirovich Kozyrev  <ivanesxxx@gmail.com>
 * Created on: Nov 19, 2021
 * Software:  STM32CubeIDE
 * Hardware:  all stm32 with hardware spi
*******************************************************************************/
#ifndef AD9833_AD9833_H_
#define AD9833_AD9833_H_
#include <math.h>
#include "stm32h7xx_hal.h"

// ------------------------- Defines -------------------------
//#define FMCLK 25000000        // Master Clock On AD9833
#define FMCLK 1e7        // Master Clock On AD9833
#define FREQUENCYFACTOR 0x10000000 //2^28

//macro for control block
#define SINWAVE 				0x0000
#define SQUAREWAVE 				0x0028
#define TRIANGLEWAVE 			0x0002
#define RESETBIT 				0x0100
#define CONTROLBITTWOFREQUENCY 	0x2000
#define CONTROLBITMSB 			0x0800
#define CONTROLBITLSB 			0x0000
#define FSELECTZERO 			0x0000
#define FSELECTONE	 			0x0800
#define PSELECTZERO 			0x0000
#define PSELECTONE	 			0x0400

//macro for setting
#define FREQZEROREGBITS			0x4000
#define FREQONEREGBITS			0x8000
#define PHASEZEROREGBITS		0xc000
#define PHASEONEREGBITS			0xe000

//sleep12
#define SLEEPTWOON				0x0040
#define SLEEPTWOOFF				0x0000


enum WaveType{SIN, SQR, TRI}; // Wave Selection

extern SPI_HandleTypeDef hspi1;//used spi interface

// ------------------ Functions  ---------------------
void AD9833_SetWave(uint8_t Wave);//setting wave type, sine, triangular, square
void AD9833_SetPhase(uint16_t Phase);//phase shift in degrees
void AD9833setFrequency(uint32_t frequency);//setting the frequency
void AD9833reset(void);//reset

#endif/* AD9833_AD9833_H_ */
