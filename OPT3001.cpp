/**
 ******************************************************************************
 * @file    OPT3001.cpp
 * @author  Oleg Artamonov
 * @version V1.0.0
 * @date    16 August 2018
 * @brief   OPT3001 class implementation
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
#include "OPT3001.h"

OPT3001::OPT3001(PinName sda, PinName scl, char slave_adr)
    :
    i2c_p(new I2C(sda, scl)), 
    i2c(*i2c_p),
    address(slave_adr)
{
    initialize();
}

OPT3001::~OPT3001()
{
    if (NULL != i2c_p)
        delete  i2c_p;
}

int OPT3001::initialize() {
    uint16_t data;
    
    char reg = OPT3001_REG_CHIP_ID;
    i2c.write(address, &reg, 1);
    int b = i2c.read(address, (char *)&data, 2, 0);
    uint16_t deviceID = (data << 8) | (data >> 8);
    
    if(deviceID != OPT3001_CHIP_ID) {
        return -1;
    }
    
    reg = OPT3001_REG_CONFIG;
    i2c.write(address, &reg, 1);
    i2c.read(address, (char *)&data, 2, 0);
    
    return 0;
}
    

int OPT3001::readSensor()
{
    uint16_t data;
    char tmp[3];
    
    /* start measurement */
    tmp[0] = OPT3001_REG_CONFIG;
    tmp[1] = OPT3001_CFG & 0xFF;
    tmp[2] = OPT3001_CFG >> 8;
    
    i2c.write(address, tmp, 3);
    
    /* wait till measurement is finished */
    int i = 100;
    do {
        /* 10 ms delay */
        wait_ms(10);

        tmp[0] = OPT3001_REG_CONFIG;
        i2c.write(address, tmp, 1);
        i2c.read(address, (char *)&data, 2, 0);
        
        if (data & OPT3001_CFG_CRF) {
            break;
        }

        i--;
    } while (i);
    
    if (i == 0) {
        return -1;
    }

    /* read result */
    tmp[0] = OPT3001_REG_RESULT;
    i2c.write(address, tmp, 1);
    i2c.read(address, (char *)&data, 2, 0);
        
    /* swap bytes, as OPT3001 sends MSB first and I2C driver expects LSB first */
    data = (data << 8) | (data >> 8);

    /* calculate lux per LSB */
    uint8_t exp = (data >> 12) & 0x0F;
    uint32_t lsb_size_x100 = (1 << exp);
    
    /* remove 4-bit exponent and leave 12-bit mantissa only */
    data &= 0x0FFF;
    
    return (((int)data * lsb_size_x100) / 100);
}
