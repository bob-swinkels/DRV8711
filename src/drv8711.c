/*
 * drv8711.c
 *
 *  Created on: 24 sep. 2021
 *      Author: Bob Swinkels
 */

#include "drv8711.h"
#include "device.h"

/**
 * Write data over SPI to the DRV8711
 * @param base  Base address of the SPI port
 * @param data  Data to be written over SPI
 */
void DRV8711_SPI_write(uint32_t base, uint16_t data){
    GPIO_writePin(PIN_SPI_CS, 1);
    SPI_writeDataBlockingNonFIFO(base, data);
    SPI_readDataBlockingNonFIFO(base);
    GPIO_writePin(PIN_SPI_CS, 0);
}

/**
 * Request data over SPI from DRV8711 and return this data
 * @param base  Base address of the SPI port
 * @param data  Address to request data from
 * @return      Data returned by the DRV8711
 */
uint16_t DRV8711_SPI_read(uint32_t base, uint16_t data){
    uint16_t return_data;

    GPIO_writePin(PIN_SPI_CS, 1);
    SPI_writeDataBlockingNonFIFO(base, data);
    return_data = SPI_readDataBlockingNonFIFO(base);
    GPIO_writePin(PIN_SPI_CS, 0);
    return return_data;
}

/**
 * Write data to the CTRL register of the DRV8711
 * @param base    Base address of the SPI port
 * @param config  Configuration of the register, is the logical OR of several values:
 *                  - #DRV8711_DISABLE_MOTOR or #DRV8711_ENABLE_MOTOR to respectively disable or enable the motor.
 *                  - #DRV8711_NORMAL_DIR_PIN or #DRV8711_INVERT_DIR_PIN to invert the direction set by the dir pin.
 *                  - #DRV8711_ADVANCE_STEP to make the indexer advance one step.
 *                  - #DRV8711_MODE_FULL_STEP, #DRV8711_MODE_HALF_STEP, #DRV8711_MODE_1_4_STEP, #DRV8711_MODE_1_8_STEP, #DRV8711_MODE_1_16_STEP, #DRV8711_MODE_1_32_STEP, #DRV8711_MODE_1_64_STEP, #DRV8711_MODE_1_128_STEP or #DRV8711_MODE_1_256_STEP to set the step-size.
 *                  - #DRV8711_INTERNAL_STALL_DETECT or #DRV8711_EXTERNAL_STALL_DETECT to set if stall is detected internally or externally.
 *                  - #DRV8711_ISENSE_GAIN_5, #DRV8711_ISENSE_GAIN_10, #DRV8711_ISENSE_GAIN_20 or #DRV8711_ISENSE_GAIN_40 to set the ISENSE amplifier gain.
 *                  - #DRV8711_DTIME_400NS, #DRV8711_DTIME_450NS, #DRV8711_DTIME_650NS or #DRV8711_DTIME_850NS to set the dead time.
 */
void DRV8711_setCtrlConfig(uint32_t base, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | CTRL_REG | config);
}

/**
 * Write data to the TORQUE register of the DRV8711
 * @param base         Base address of the SPI port
 * @param current_out  Sets full-scale output current for both H-bridges
 * @param config       Configuration of the register, is the logical OR of several values:
 *                       - #DRV8711_BEMF_SAMPLE_TRESHOLD_50US, #DRV8711_BEMF_SAMPLE_TRESHOLD_100US, #DRV8711_BEMF_SAMPLE_TRESHOLD_200US, #DRV8711_BEMF_SAMPLE_TRESHOLD_300US, #DRV8711_BEMF_SAMPLE_TRESHOLD_400US, #DRV8711_BEMF_SAMPLE_TRESHOLD_600US, #DRV8711_BEMF_SAMPLE_TRESHOLD_800US or #DRV8711_BEMF_SAMPLE_TRESHOLD_1000US to set the back EMF sample threshold.
 */
void DRV8711_setTorqueConfig(uint32_t base, unsigned char current_out, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | TORQUE_REG | current_out | config);
}

/**
 * Write data to the OFF register of the DRV8711
 * @param base      Base address of the SPI port
 * @param off_time  Sets fixed off time, in increments of 500 ns
 *                    - `0x00` = 500 ns
 *                    - `0xFF` = 128 us
 * @param config    Configuration of the register, is the logical OR of several values:
 *                    - #DRV8711_INTERNAL_INDEXER or #DRV8711_BYPASS_INDEXER to use or bypass the internal indexer.
 */
void DRV8711_setOffConfig(uint32_t base, unsigned char off_time, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | OFF_REG | off_time | config);
}

/**
 * Write data to the BLANK register of the DRV8711
 * @param base        Base address of the SPI port
 * @param blank_time  Sets current trip blanking time, in increments of 20 ns. Also sets minimum on-time of PWM.
 *                      - `0x00`: 1 us
 *                      - ...
 *                      - `0x32`: 1 us
 *                      - `0x33`: 1.02 us
 *                      - ...
 *                      - `0xFE`: 5.10 us
 *                      - `0xFF`: 5.12 us
 * @param config      Configuration of the register, is the logical OR of several values:
 *                      - #DRV8711_DISABLE_ADAPTIVE_BLANKING or #DRV8711_ENABLE_ADAPTIVE_BLANKING to enable/disable adaptive blanking time.
 */
void DRV8711_setBlankConfig(uint32_t base, unsigned char blank_time, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | BLANK_REG | blank_time | config);
}

/**
 * Write data to the DECAY register of the DRV8711
 * @param base        Base address of the SPI port
 * @param decay_time  Sets mixed decay transition time, in increments of 500 ns.
 *                      - `0x00` = 500 ns
 *                      - `0xFF` = 128 us
 * @param config      Configuration of the register, is the logical OR of several values:
 *                      - #DRV8711_SLOW_DACAY, #DRV8711_SLOW_INCR_FAST_DECR, #DRV8711_FAST_DECAY, #DRV8711_MIXED_DECAY, #DRV8711_SLOW_INCR_AUTOMIXED_DECR or #DRV8711_AUTOMIXED_DECAY to set the decay mode.
 */
void DRV8711_setDecayConfig(uint32_t base, unsigned char decay_time, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | DECAY_REG | decay_time | config);
}

/**
 * Write data to the STALL register of the DRV8711
 * @param base            Base address of the SPI port
 * @param stall_treshold  Sets stall detect threshold. The correct setting needs to be determined experimentally.
 * @param config          Configuration of the register, is the logical OR of several values:
 *                          - #DRV8711_STALLN_ASSERT_1STEP, #DRV8711_STALLN_ASSERT_2STEP, #DRV8711_STALLN_ASSERT_4STEP, #DRV8711_STALLN_ASSERT_8STEP to set when STALLn is asserted.
 *                          - #DRV8711_VDIV_32, #DRV8711_VDIV_16, #DRV8711_VDIV_8 or #DRV8711_VDIV_4 to set the division of back EMF.
 */
void DRV8711_setStallConfig(uint32_t base, unsigned char stall_treshold, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | STALL_REG | stall_treshold | config);
}

/**
 * Write data to the DRIVE register of the DRV8711
 * @param base    Base address of the SPI port
 * @param config  Configuration of the register, is the logical OR of several values:
 *                  - #DRV8711_OCP_TRESHOLD_250MV, #DRV8711_OCP_TRESHOLD_500MV, #DRV8711_OCP_TRESHOLD_750MV or #DRV8711_OCP_TRESHOLD_1000MV to set the OCP threshold.
 *                  - #DRV8711_OCP_DEGLITCH_TIME_1US, #DRV8711_OCP_DEGLITCH_TIME_2US, #DRV8711_OCP_DEGLITCH_TIME_4US or #DRV8711_OCP_DEGLITCH_TIME_8US to set the OCP deglitch time.
 *                  - #DRV8711_LOW_DRIVE_TIME_250NS, #DRV8711_LOW_DRIVE_TIME_500NS, #DRV8711_LOW_DRIVE_TIME_1000NS or #DRV8711_LOW_DRIVE_TIME_2000NS to set the low-side gate drive time.
 *                  - #DRV8711_HIGH_DRIVE_TIME_250NS, #DRV8711_HIGH_DRIVE_TIME_500NS, #DRV8711_HIGH_DRIVE_TIME_1000NS or #DRV8711_HIGH_DRIVE_TIME_2000NS to set the high-side gate drive time.
 *                  - #DRV8711_LOW_PEAK_CURRENT_100MA, #DRV8711_LOW_PEAK_CURRENT_200MA, #DRV8711_LOW_PEAK_CURRENT_300MA or #DRV8711_LOW_PEAK_CURRENT_400MA to set the low-side gate drive peak current.
 *                  - #DRV8711_HIGH_PEAK_CURRENT_100MA, #DRV8711_HIGH_PEAK_CURRENT_200MA, #DRV8711_HIGH_PEAK_CURRENT_300MA or #DRV8711_HIGH_PEAK_CURRENT_400MA to set the high-side gate drive peak current.
 */
void DRV8711_setDriveConfig(uint32_t base, uint16_t config) {
    DRV8711_SPI_write(base, REGWRITE | DRIVE_REG | config);
}

/**
 * Returns contents from the DRV8711 status register
 * A 0 has to be written to the bits of the status flags to clear them.
 * @param base  Base address of the SPI port
 * @return      This function returns the contents of the status register. The value it returns is an OR of the following values:
 *                - #DRV8711_STS_OVERTEMPERATURE_SHUTDOWN Device has entered overtemperature shutdown.
 *                - #DRV8711_STS_A_OVERCURRENT_SHUTDOWN Channel A overcurrent shutdown.
 *                - #DRV8711_STS_B_OVERCURRENT_SHUTDOWN Channel B overcurrent shutdown.
 *                - #DRV8711_STS_A_PREDRIVER_FAULT Channel A predriver fault.
 *                - #DRV8711_STS_B_PREDRIVER_FAULT Channel B predriver fault.
 *                - #DRV8711_STS_UNDERVOLTAGE_LOCKOUT Undervoltage lockout.
 *                - #DRV8711_STS_STALL_DETECTED Stall detected.
 *                - #DRV8711_STS_LATCHED_STALL_DETECTED Latched stall detected.
 */
uint16_t DRV8711_getStatus(uint32_t base) {
    return DRV8711_SPI_read(base, REGREAD | STATUS_REG);
}

/**
 * Clears selected fields of the DRV8711 status register
 * @param base         Base address of the SPI port
 * @param statusFlags  Bit mask of the status flags to be cleared.
 *                       - #DRV8711_STS_OVERTEMPERATURE_SHUTDOWN Device has entered overtemperature shutdown
 *                       - #DRV8711_STS_A_OVERCURRENT_SHUTDOWN Channel A overcurrent shutdown
 *                       - #DRV8711_STS_B_OVERCURRENT_SHUTDOWN Channel B overcurrent shutdown
 *                       - #DRV8711_STS_A_PREDRIVER_FAULT Channel A predriver fault.
 *                       - #DRV8711_STS_B_PREDRIVER_FAULT Channel B predriver fault.
 *                       - #DRV8711_STS_UNDERVOLTAGE_LOCKOUT Undervoltage lockout.
 *                       - #DRV8711_STS_STALL_DETECTED Stall detected.
 *                       - #DRV8711_STS_LATCHED_STALL_DETECTED Latched stall detected.
 */
void DRV8711_clearStatus(uint32_t base, uint16_t statusFlags) {
    DRV8711_SPI_write(base, REGWRITE | STATUS_REG | (0xFF & ~statusFlags));
}
