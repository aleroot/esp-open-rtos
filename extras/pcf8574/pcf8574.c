/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "pcf8574.h"
#include <i2c/i2c.h>

uint8_t pcf8574_port_read(uint8_t addr)
{
    i2c_start();
    uint8_t res = i2c_write((addr << 1) | 1) ? i2c_read(1) : 0;
    i2c_stop();
    return res;
}

size_t pcf8574_port_read_buf(uint8_t addr, void *buf, size_t len)
{
    if (!len || !buf) return 0;
    uint8_t *_buf = (uint8_t *)buf;

    i2c_start();
    if (!i2c_write((addr << 1) | 1)) return 0;
    for (size_t i = 0; i < len; i++)
        *_buf++ = i2c_read(i == len - 1);
    i2c_stop();
    return len;
}

size_t pcf8574_port_write_buf(uint8_t addr, void *buf, size_t len)
{
    if (!len || !buf) return 0;
    uint8_t *_buf = (uint8_t *)buf;

    i2c_start();
    if (!i2c_write(addr << 1)) return 0;
    for (size_t i = 0; i < len; i++)
        i2c_write(*_buf++);
    return len;
}

void pcf8574_port_write(uint8_t addr, uint8_t value)
{
    i2c_start();
    if (i2c_write(addr << 1)) i2c_write(value);
    i2c_stop();
}

bool pcf8574_gpio_read(uint8_t addr, uint8_t num)
{
    return (bool)((pcf8574_port_read(addr) >> num) & 1);
}

void pcf8574_gpio_write(uint8_t addr, uint8_t num, bool value)
{
    uint8_t bit = (uint8_t)value << num;
    uint8_t mask = ~(1 << num);
    pcf8574_port_write (addr, (pcf8574_port_read(addr) & mask) | bit);
}
