/*
 * drv8711.h
 *
 *  Created on: 24 sep. 2021
 *      Author: Bob Swinkels
 */

#ifndef DRV8711_H_
#define DRV8711_H_

/*****************************************************************************/

// Includes
#include "driverlib.h"

/*****************************************************************************/

#define PIN_SPI_CS  57 ///< Pin for SPI chip select

// Defines
#define REGWRITE (0x00 << 15) ///< Write to register
#define REGREAD  (0x01 << 15) ///< Read from register

#define CTRL_REG   (0x00 << 12) ///< Address for CTRL register
#define TORQUE_REG (0x01 << 12) ///< Address for TORQUE register
#define OFF_REG    (0x02 << 12) ///< Address for OFF register
#define BLANK_REG  (0x03 << 12) ///< Address for BLANK register
#define DECAY_REG  (0x04 << 12) ///< Address for DECAY register
#define STALL_REG  (0x05 << 12) ///< Address for STALL register
#define DRIVE_REG  (0x06 << 12) ///< Address for DRIVE register
#define STATUS_REG (0x07 << 12) ///< Address for STATUS register

#define DRV8711_DISABLE_MOTOR (0x00 << 0) ///< Disable motor (default)
#define DRV8711_ENABLE_MOTOR  (0x01 << 0) ///< Enable motor

#define DRV8711_NORMAL_DIR_PIN (0x00 << 1) ///< Direction set by DIR pin (default)
#define DRV8711_INVERT_DIR_PIN (0x01 << 1) ///< Direction set by inverse of DIR pin

#define DRV8711_ADVANCE_STEP (0x01 << 2) ///< Indexer will advance one step; automatically cleared after write

#define DRV8711_MODE_FULL_STEP  (0x00 << 3) ///< Full-step, 71% current
#define DRV8711_MODE_HALF_STEP  (0x01 << 3) ///< Half-step
#define DRV8711_MODE_1_4_STEP   (0x02 << 3) ///< 1/4 step (default)
#define DRV8711_MODE_1_8_STEP   (0x03 << 3) ///< 1/8 step
#define DRV8711_MODE_1_16_STEP  (0x04 << 3) ///< 1/16 step
#define DRV8711_MODE_1_32_STEP  (0x05 << 3) ///< 1/32 step
#define DRV8711_MODE_1_64_STEP  (0x06 << 3) ///< 1/64 step
#define DRV8711_MODE_1_128_STEP (0x07 << 3) ///< 1/128 step
#define DRV8711_MODE_1_256_STEP (0x08 << 3) ///< 1/256 step

#define DRV8711_INTERNAL_STALL_DETECT (0x00 << 7) ///< Internal stall detect (default)
#define DRV8711_EXTERNAL_STALL_DETECT (0x01 << 7) ///< External stall detect

#define DRV8711_ISENSE_GAIN_5  (0x00 << 8) ///< ISENSE amplifier gain 5 (default)
#define DRV8711_ISENSE_GAIN_10 (0x01 << 8) ///< ISENSE amplifier gain 10
#define DRV8711_ISENSE_GAIN_20 (0x02 << 8) ///< ISENSE amplifier gain 20
#define DRV8711_ISENSE_GAIN_40 (0x03 << 8) ///< ISENSE amplifier gain 40

#define DRV8711_DTIME_400NS (0x00 << 10) ///< 400 ns dead time
#define DRV8711_DTIME_450NS (0x01 << 10) ///< 450 ns dead time
#define DRV8711_DTIME_650NS (0x02 << 10) ///< 650 ns dead time
#define DRV8711_DTIME_850NS (0x03 << 10) ///< 850 ns dead time (default)

#define DRV8711_BEMF_SAMPLE_TRESHOLD_50US   (0x00 << 8) ///< Back EMF sample threshold 50 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_100US  (0x01 << 8) ///< Back EMF sample threshold 100 us (default)
#define DRV8711_BEMF_SAMPLE_TRESHOLD_200US  (0x02 << 8) ///< Back EMF sample threshold 200 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_300US  (0x03 << 8) ///< Back EMF sample threshold 300 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_400US  (0x04 << 8) ///< Back EMF sample threshold 400 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_600US  (0x05 << 8) ///< Back EMF sample threshold 600 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_800US  (0x06 << 8) ///< Back EMF sample threshold 800 us
#define DRV8711_BEMF_SAMPLE_TRESHOLD_1000US (0x07 << 8) ///< Back EMF sample threshold 1000 us

#define DRV8711_INTERNAL_INDEXER (0x00 << 8) ///< Use internal indexer (default)
#define DRV8711_BYPASS_INDEXER   (0x01 << 8) ///< Bypass indexer, use xINx inputs to control outputs

#define DRV8711_DISABLE_ADAPTIVE_BLANKING (0x00 << 8) ///< Disable adaptive blanking time (default)
#define DRV8711_ENABLE_ADAPTIVE_BLANKING  (0x01 << 8) ///< Enable adaptive blanking time

#define DRV8711_SLOW_DACAY               (0x00 << 8) ///< Force slow decay at all times
#define DRV8711_SLOW_INCR_FAST_DECR      (0x01 << 8) ///< Slow decay for increasing current, mixed decay for decreasing current (indexer mode only) (default)
#define DRV8711_FAST_DECAY               (0x02 << 8) ///< Force fast decay at all times
#define DRV8711_MIXED_DECAY              (0x03 << 8) ///< Use mixed decay at all times
#define DRV8711_SLOW_INCR_AUTOMIXED_DECR (0x04 << 8) ///< Slow decay for increasing current, auto mixed decay for decreasing current (indexer mode only)
#define DRV8711_AUTOMIXED_DECAY          (0x05 << 8) ///< Use auto mixed decay at all times

#define DRV8711_STALLN_ASSERT_1STEP (0x00 << 8) ///< STALLn asserted on first step with back EMF below SDTHR (default)
#define DRV8711_STALLN_ASSERT_2STEP (0x01 << 8) ///< STALLn asserted after 2 steps
#define DRV8711_STALLN_ASSERT_4STEP (0x02 << 8) ///< STALLn asserted after 4 steps
#define DRV8711_STALLN_ASSERT_8STEP (0x03 << 8) ///< STALLn asserted after 5 steps

#define DRV8711_VDIV_32 (0x00 << 10) ///< Back EMF is divided by 32 (default)
#define DRV8711_VDIV_16 (0x01 << 10) ///< Back EMF is divided by 16
#define DRV8711_VDIV_8  (0x02 << 10) ///< Back EMF is divided by 8
#define DRV8711_VDIV_4  (0x03 << 10) ///< Back EMF is divided by 4

#define DRV8711_OCP_TRESHOLD_250MV  (0x00 << 0) ///< OCP threshold 250 mV
#define DRV8711_OCP_TRESHOLD_500MV  (0x01 << 0) ///< OCP threshold 500 mV (default)
#define DRV8711_OCP_TRESHOLD_750MV  (0x02 << 0) ///< OCP threshold 750 mV
#define DRV8711_OCP_TRESHOLD_1000MV (0x03 << 0) ///< OCP threshold 1000 mV

#define DRV8711_OCP_DEGLITCH_TIME_1US (0x00 << 2) ///< OCP deglitch time 1 us
#define DRV8711_OCP_DEGLITCH_TIME_2US (0x01 << 2) ///< OCP deglitch time 2 us
#define DRV8711_OCP_DEGLITCH_TIME_4US (0x02 << 2) ///< OCP deglitch time 4 us (default)
#define DRV8711_OCP_DEGLITCH_TIME_8US (0x03 << 2) ///< OCP deglitch time 8 us

#define DRV8711_LOW_DRIVE_TIME_250NS  (0x00 << 4) ///< Low-side gate drive time 250 ns
#define DRV8711_LOW_DRIVE_TIME_500NS  (0x01 << 4) ///< Low-side gate drive time 500 ns (default)
#define DRV8711_LOW_DRIVE_TIME_1000NS (0x02 << 4) ///< Low-side gate drive time 1 us
#define DRV8711_LOW_DRIVE_TIME_2000NS (0x03 << 4) ///< Low-side gate drive time 2 us

#define DRV8711_HIGH_DRIVE_TIME_250NS  (0x00 << 6) ///< High-side gate drive time 250 ns
#define DRV8711_HIGH_DRIVE_TIME_500NS  (0x01 << 6) ///< High-side gate drive time 500 ns (default)
#define DRV8711_HIGH_DRIVE_TIME_1000NS (0x02 << 6) ///< High-side gate drive time 1 us
#define DRV8711_HIGH_DRIVE_TIME_2000NS (0x03 << 6) ///< High-side gate drive time 2 us

#define DRV8711_LOW_PEAK_CURRENT_100MA (0x00 << 8) ///< Low-side gate drive 100 mA peak (sink) current
#define DRV8711_LOW_PEAK_CURRENT_200MA (0x01 << 8) ///< Low-side gate drive 200 mA peak (sink) current
#define DRV8711_LOW_PEAK_CURRENT_300MA (0x02 << 8) ///< Low-side gate drive 300 mA peak (sink) current (default)
#define DRV8711_LOW_PEAK_CURRENT_400MA (0x03 << 8) ///< Low-side gate drive 400 mA peak (sink) current

#define DRV8711_HIGH_PEAK_CURRENT_50MA (0x00 << 8) ///< High-side gate drive 50 mA peak (source) current
#define DRV8711_HIGH_PEAK_CURRENT_100MA (0x01 << 8) ///< High-side gate drive 100 mA peak (source) current
#define DRV8711_HIGH_PEAK_CURRENT_150MA (0x02 << 8) ///< High-side gate drive 150 mA peak (source) current
#define DRV8711_HIGH_PEAK_CURRENT_200MA (0x03 << 8) ///< High-side gate drive 200 mA peak (source) current

#define DRV8711_STS_OVERTEMPERATURE_SHUTDOWN (0x01 << 0) ///< Device has entered overtemperature shutdown bitmask.
#define DRV8711_STS_A_OVERCURRENT_SHUTDOWN   (0x01 << 1) ///< Channel A overcurrent shutdown bitmask.
#define DRV8711_STS_B_OVERCURRENT_SHUTDOWN   (0x01 << 2) ///< Channel B overcurrent shutdown bitmask.
#define DRV8711_STS_A_PREDRIVER_FAULT        (0x01 << 3) ///< Channel A predriver fault bitmask.
#define DRV8711_STS_B_PREDRIVER_FAULT        (0x01 << 4) ///< Channel B predriver fault bitmask.
#define DRV8711_STS_UNDERVOLTAGE_LOCKOUT     (0x01 << 5) ///< Undervoltage lockout bitmask.
#define DRV8711_STS_STALL_DETECTED           (0x01 << 6) ///< Stall detected bitmask.
#define DRV8711_STS_LATCHED_STALL_DETECTED   (0x01 << 7) ///< Latched stall detected bitmask.

/*****************************************************************************/

// Function declarations
void DRV8711_SPI_write(uint32_t base, uint16_t data);
uint16_t DRV8711_SPI_read(uint32_t base, uint16_t data);
void DRV8711_setCtrlConfig(uint32_t base, uint16_t config);
void DRV8711_setTorqueConfig(uint32_t base, unsigned char current_out, uint16_t config);
void DRV8711_setOffConfig(uint32_t base, unsigned char off_time, uint16_t config);
void DRV8711_setBlankConfig(uint32_t, unsigned char blank_time, uint16_t config);
void DRV8711_setDecayConfig(uint32_t base, unsigned char decay_time, uint16_t config);
void DRV8711_setStallConfig(uint32_t base, unsigned char stall_treshold, uint16_t config);
void DRV8711_setDriveConfig(uint32_t base, uint16_t config);
uint16_t DRV8711_getStatus(uint32_t base);
void DRV8711_clearStatus(uint32_t base, uint16_t statusFlags);

#endif /* DRV8711_H_ */
