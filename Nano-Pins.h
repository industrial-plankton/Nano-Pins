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
#include <Setttings.h>
// Comment out if you arent using any of the Special defines and dont have the file.
// So platformIO will build with the corrent include paths ensure you add Settings.h to the /include Directort and add the following to platformi.ini
/* 
build_flags =
    -I include/
    */
#ifndef NonNano
#include <avr/io.h>
#endif

// Calculate PWM value from percentage, passed as int to avoid overflow
unsigned char calcPWM(int val, int MaxValue);

// Digital Pin Control
class Pin
{
protected:
#ifndef NonNano
    unsigned char ioAdd; // (ioAdd = PIN, +1 = DDR, +2 = PORT) aka (read,mode,output)
    #endif
    unsigned char PinNum;

public:
    Pin(unsigned char PinNum, unsigned char mode) : PinNum{PinNum}
    {
        #ifndef NonNano
        if (14 <= PinNum && PinNum <= 19) //A0-A5
        {
            this->ioAdd = 0x06;
            this->PinNum -= 14;
            this->PinNum = _BV(this->PinNum);
        }
        else if (8 <= PinNum && PinNum <= 13) //D8-D13
        {
            this->ioAdd = 0x03;
            this->PinNum -= 8;
            this->PinNum = _BV(this->PinNum);
        }
        else
        { //D0-D7
            this->ioAdd = 0x09;
            this->PinNum = _BV(this->PinNum);
        }
        #endif
        this->Low();
        pinMode(PinNum, mode);
    }

    void Set(bool val);
    void High();
    void Low();
    unsigned char Read();
};

// Analog Pin Control
class AnPin
{
protected:
    const unsigned char pin;
    int val;
    int MaxValue;

public:
    AnPin(const unsigned char pin) : pin{pin}
    {
        pinMode(pin, OUTPUT);
        MaxValue = 100;
    }

    void Set(unsigned char val);
    int Get();
    int SetMaxValue(int newMax = 0);
};

#endif