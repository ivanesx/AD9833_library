/***************************************************************************//**
 * File	AD9833.c
 * Title	AD9833 Wave Generator for STM32 HAL
 * Author:    Ivan Vladimirovich Kozyrev  <ivanesxxx@gmail.com>
 * Created on: Nov 19, 2021
 * Software:  STM32CubeIDE
 * Hardware:  all stm32 with hardware spi
*******************************************************************************/
#include "AD9833.h"
// ------------------- Variables ----------------
uint16_t FRQLW = 0;    			// FREQ REG LSB
uint16_t FRQHW = 0;    			// FREQ REG MSB
uint32_t  phaseVal=0;  			// Phase Value
uint8_t wave_value=0;  			// Wave type
uint16_t last_phase_val =0;		// Phase value in degree
uint32_t FreqWord =0;			// Value Frequency Register
// -------------------------------- Functions --------------------------------

/**
 * @brief Sends data ad9833
 * @param[in]	data data to send
 * @return none.
 */
void WriteRegister(int16_t data) {

	HAL_SPI_Transmit (&hspi1, (uint8_t*) &data, 1, 500);

}

/**
 * @brief Selection of the type of output signal - sine, triangle, square
 * @param[in]	Wave wave type value from 0 to 2
 * @return none.
 */
void AD9833_SetWave(uint8_t Wave){
  switch(Wave){
  case 0:
	WriteRegister(SINWAVE);
	wave_value=0;
    break;
  case 1:
	WriteRegister(SQUAREWAVE);
	wave_value=1;
    break;
  case 2:
	WriteRegister(TRIANGLEWAVE);
	wave_value=2;
    break;
  default:
    break;
  }
}

/**
 * @brief Shifts the phase by the required value in degrees. PHAS0 register
 * @param[in]	Phase the value of the phase shift relative to the current in degrees
 * @return none.
 */
void AD9833_SetPhase(uint16_t Phase){
	last_phase_val += Phase;
	if(last_phase_val > 360)last_phase_val -= 360;
	phaseVal = (uint32_t)(last_phase_val * 4096/360);
	//Set control bits PHASE0 or PHASE1
	WriteRegister(phaseVal|PHASEZEROREGBITS);

}

/**
 * @brief Sets the frequency value in Hz.FREQ0 register, 28 bits
 * @param[in]	frequency set frequency in Hz
 * @return none.
 */
void AD9833setFrequency(uint32_t frequency) {

	FreqWord = (uint64_t)((uint64_t)frequency * FREQUENCYFACTOR) / FMCLK;
	FRQHW = (int16_t)(FreqWord >> 14);
	//FRQLW = (int16_t)(FreqWord & 0x0fff);
	FRQLW = (int16_t)(FreqWord & 0x3fff);

  //Set control bits FREQ0 or FREQ1
	FRQLW |= FREQZEROREGBITS;
	FRQHW |= FREQZEROREGBITS;
	//setting the frequency length (28 bit or 14) and wave type
  switch(wave_value){
  	      case 0:
  	      	WriteRegister(CONTROLBITTWOFREQUENCY|SINWAVE);
  	        break;
  	      case 1:
  	      	WriteRegister(CONTROLBITTWOFREQUENCY|SQUAREWAVE);
  	      	break;
  	      case 2:
  	      	WriteRegister(CONTROLBITTWOFREQUENCY|TRIANGLEWAVE);
  	      	break;
  	      default:
  	        break;
  	      }
  WriteRegister(FRQLW);  // Write lower 14 bits
  WriteRegister(FRQHW);  // Write upper 14 bits

    }

/**
 * @brief The reset function resets appropriate internal registers to 0
 * to provide an analog output of midscale
 * @param[in]	none
 * @return none.
 */
void AD9833reset(void) {
  WriteRegister(RESETBIT);
}




