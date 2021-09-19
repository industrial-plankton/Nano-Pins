#ifndef NANO_PINS_H
#define NANO_PINS_H

/*
MIT License

Copyright (c) 2021 industrial-plankton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <avr/io.h>

// Calculate PWM value from percentage, passed as int to avoid overflow
byte calcPWM(int val, int MaxValue);

// Digital Pin Control
class Pin
{
    // Fully Tested as of 21-04-13
protected:
    uint8_t ioAdd; // (ioAdd = PIN, +1 = DDR, +2 = PORT) aka (read,mode,output)
    uint8_t PinNum;

public:
    Pin(uint8_t PinNum, uint8_t mode) : PinNum{PinNum}
    {
        if (14 <= PinNum && PinNum <= 19) //A0-A5
        {
            this->ioAdd = 0x06;
            this->PinNum -= 14;
        }
        else if (8 <= PinNum && PinNum <= 13) //D8-D13
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

// Analog Pin Control
class AnPin
{
protected:
    const byte pin;
    int val;
    int MaxValue;
public:
    AnPin(const byte pin) : pin{pin}
    {
        pinMode(pin, OUTPUT);
        MaxValue = 100;
    }

    void Set(byte val)
    {
        if (this->val != val)
        {
            this->val = val;
            analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
        }
    }

    int Get()
    {
        return val;
    }

    int SetMaxValue(int newMax = 0){
        if (newMax != 0 && MaxValue != newMax){
            this->MaxValue = newMax;
            analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
        }
        return this->MaxValue;
    }
};

#endif