/**
 ******************************************************************************
 * @file    OPT3001.h
 * @author  Oleg Artamonov
 * @version V1.0.0
 * @date    16 August 2018
 * @brief   This file contains the class of a OPT3001 ambient light sensor library
 ******************************************************************************
 * @attention
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mbed.h"

#ifndef MBED_OPT3001_H
#define MBED_OPT3001_H

// default I2C address with address pin tied to GND
static const int OPT3001_DEFAULT_ADDRESS = 0x44 << 1;

static const uint16_t OPT3001_CFG_FC_1    = 0x0000;
static const uint16_t OPT3001_CFG_FC_2    = 0x0100;
static const uint16_t OPT3001_CFG_FC_4    = 0x0200;
static const uint16_t OPT3001_CFG_FC_8    = 0x0300;
static const uint16_t OPT3001_CFG_MASK    = 0x0400;
static const uint16_t OPT3001_CFG_POLPOS  = 0x0800;
static const uint16_t OPT3001_CFG_LATCH   = 0x1000;
static const uint16_t OPT3001_CFG_FLAGL   = 0x2000;
static const uint16_t OPT3001_CFG_FLAGH   = 0x4000;
static const uint16_t OPT3001_CFG_CRF     = 0x8000;
static const uint16_t OPT3001_CFG_OVF     = 0x0001;
static const uint16_t OPT3001_CFG_SHDN    = 0x0000;
static const uint16_t OPT3001_CFG_SHOT    = 0x0002;
static const uint16_t OPT3001_CFG_CONT    = 0x0004;
static const uint16_t OPT3001_CFG_100MS   = 0x0000;
static const uint16_t OPT3001_CFG_800MS   = 0x0008;
static const uint16_t OPT3001_CFG_RNAUTO  = 0x00C0;

static const uint16_t OPT3001_CFG = (OPT3001_CFG_FC_1 | OPT3001_CFG_SHOT | OPT3001_CFG_100MS | OPT3001_CFG_RNAUTO );
static const uint16_t OPT3001_CFG_DEFAULT = 0x10C8;

static const uint8_t OPT3001_REG_RESULT         = 0x00;
static const uint8_t OPT3001_REG_CONFIG         = 0x01;
static const uint8_t OPT3001_REG_MAN_ID         = 0x7E;
static const uint8_t OPT3001_REG_CHIP_ID        = 0x7F;

static const uint16_t OPT3001_MAN_ID            = 0x5449;
static const uint16_t OPT3001_CHIP_ID           = 0x3001;
static const uint16_t OPT3001_REG_CONFIG_MASK   = 0xFE1F;

class OPT3001
{
public:

    /** Create an OPT3001 instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param sda I2C-bus SDA pin
     * @param scl I2C-bus SCL pin
     * @param slave_adr (option) I2C-bus address (default: 0x47)
     */
    OPT3001(PinName sda, PinName sck, char slave_adr = OPT3001_DEFAULT_ADDRESS);

    /** Create a OPT3001  instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param i2c_obj I2C object (instance)
     * @param slave_adr (option) I2C-bus address (default: 0x47)
     */
    OPT3001(I2C &i2c_obj, char slave_adr = OPT3001_DEFAULT_ADDRESS);

    /** Destructor of OPT3001
     */
    virtual ~OPT3001();

    /** OPT3001 sensor initialization
     *
     *  Configure sensor setting and read parameters for calibration
     *
     */
    int initialize(void);

    /** Read the current value (lux) from OPT3001 sensor
     *
     */
    int readSensor(void);

private:

    I2C         *i2c_p;
    I2C         &i2c;
    char        address;
};

#endif // MBED_OPT3001_H

