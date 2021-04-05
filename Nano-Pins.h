/*
*  Copyright (C) 2020 Industrial Plankton IndustrialPlankton.com
*  
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <avr/io.h>

class Pin
{
protected:
    uint8_t ioAdd; // (ioAdd = PIN, +1 = DDR, +2 = PORT) aka (read,mode,output)
    uint8_t PinNum;

public:
    Pin(uint8_t PinNum, uint8_t mode) : PinNum{PinNum}
    {
        if (14 <= PinNum <= 21) //A0-A5
        {
            this->ioAdd = 0x06;
            this->PinNum -= 14;
        }
        else if (8 <= PinNum <= 13) //D8-D13
        {
            this->ioAdd = 0x03;
            this->PinNum -= 8;
        }
        else
        { //D0-D7
            this->ioAdd = 0x09;
        }

        this->Low();
        pinMode(PinNum, mode);
    }

    void Set(bool val)
    {
        if (val)
        {
            High();
        }
        else
        {
            Low();
        }
    }

    void High()
    {
        _SFR_IO8(this->ioAdd + 2) |= _BV(this->PinNum);
    }

    void Low()
    {
        _SFR_IO8(this->ioAdd + 2) &= ~_BV(this->PinNum);
    }

    uint8_t Read()
    {
        return _SFR_IO8(this->ioAdd) & _BV(this->PinNum);
    }
};