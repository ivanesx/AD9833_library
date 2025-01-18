/***************************************************************************//**
 * File    AD9833.c
 * Title   AD9833 Wave Generator for STM32 HAL
 * Author  Ivan Vladimirovich Kozyrev <ivanesxxx@gmail.com>
 * Created on Nov 19, 2021
 * Software STM32CubeIDE
 * Hardware All STM32 with hardware SPI
*******************************************************************************/
#include "AD9833.h"
// ------------------- Variables ----------------
static uint16_t FRQLW = 0;        // FREQ REG LSB
static uint16_t FRQHW = 0;        // FREQ REG MSB
static uint32_t phaseVal = 0;     // Phase Value
static uint8_t wave_value = 0;    // Wave type
static uint16_t last_phase_val = 0; // Phase value in degrees
static uint32_t FreqWord = 0;     // Value Frequency Register

// -------------------------------- Functions --------------------------------

/**
 * @brief Sends data to AD9833
 * @param[in] data Data to send
 * @return None.
 */
static void WriteRegister(uint16_t data) {
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, sizeof(data), HAL_MAX_DELAY) != HAL_OK) {
        // Обработка ошибки, например, можно использовать LED или UART для отладки
        Error_Handler(); // Предполагается, что функция Error_Handler() определена где-то в проекте
    }
}

/**
 * @brief Selects the type of output signal - sine, triangle, square
 * @param[in] wave Wave type value (0: sine, 1: square, 2: triangle)
 * @return None.
 */
void AD9833_SetWave(WaveType wave) {
    switch (wave) {
        case SIN:
            WriteRegister(SINWAVE);
            break;
        case SQR:
            WriteRegister(SQUAREWAVE);
            break;
        case TRI:
            WriteRegister(TRIANGLEWAVE);
            break;
        default:
            // Invalid wave type, do nothing
            break;
    }
    wave_value = wave; // Update the current wave value
    // Добавляем дополнительную проверку после записи
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&wave_value, sizeof(wave_value), HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Shifts the phase by the required value in degrees (PHAS0 register)
 * @param[in] phase The value of the phase shift in degrees
 * @return None.
 */
void AD9833_SetPhase(uint16_t phase) {
    last_phase_val = (last_phase_val + phase) % 360; // Keep within 0-360 degrees
    phaseVal = (uint32_t)(last_phase_val * 4096 / 360);
    uint16_t phaseData = phaseVal | PHASEZEROREGBITS; // Set control bits PHASE0 or PHASE1
    WriteRegister(phaseData); // Send phase data to AD9833
    // Добавляем дополнительную проверку после записи
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&phaseData, sizeof(phaseData), HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Sets the frequency value in Hz (FREQ0 register, 28 bits)
 * @param[in] frequency Frequency to set in Hz
 * @return None.
 */
void AD9833_SetFrequency(uint32_t frequency) {
    FreqWord = (uint64_t)((uint64_t)frequency * FREQUENCYFACTOR) / FMCLK;
    FRQHW = (uint16_t)(FreqWord >> 14);
    FRQLW = (uint16_t)(FreqWord & 0x3FFF);
    
    uint16_t freqControlBits = FREQZEROREGBITS | (wave_value == SIN ? SINWAVE : (wave_value == SQR ? SQUAREWAVE : TRIANGLEWAVE));
    
    // Set control bits FREQ0 or FREQ1
    WriteRegister(freqControlBits);
    WriteRegister(FRQLW);  // Write lower 14 bits
    WriteRegister(FRQHW);  // Write upper 14 bits
    
    // Добавляем дополнительные проверки после записи
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&freqControlBits, sizeof(freqControlBits), HAL_MAX_DELAY) != HAL_OK ||
        HAL_SPI_Transmit(&hspi1, (uint8_t*)&FRQLW, sizeof(FRQLW), HAL_MAX_DELAY) != HAL_OK ||
        HAL_SPI_Transmit(&hspi1, (uint8_t*)&FRQHW, sizeof(FRQHW), HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Resets appropriate internal registers to provide an analog output of midscale
 * @return None.
 */
void AD9833_Reset(void) {
    WriteRegister(RESETBIT);
    // Добавляем дополнительную проверку после записи
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&RESETBIT, sizeof(RESETBIT), HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler();
    }
}
