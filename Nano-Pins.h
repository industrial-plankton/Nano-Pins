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
// #include <Settings.h>
// Comment out if you arent using any of the Special defines and dont have the file.
// So platformIO will build with the corrent include paths ensure you add Settings.h to the /include Directort and add the following to platformi.ini
/*
build_flags =
    -I include/
    */
#ifdef ARDUINO_AVR_NANO
#include <avr/io.h>
#endif

// Calculate PWM value from percentage, passed as int to avoid overflow
unsigned char calcPWM(int val, int MaxValue);

// Port calculations to allow constriction of Pin with consts
unsigned char FindPinNumfunc(unsigned char pinnum);
unsigned char FindIOAddress(unsigned char pinnum);

// Digital Pin Control
class Pin
{
protected:
#ifdef ARDUINO_AVR_NANO
    const unsigned char ioAdd; // (ioAdd = PIN, +1 = DDR, +2 = PORT) aka (read,mode,output)
#endif
    const unsigned char PinNum;

public:
    Pin(const unsigned char PinNum, const unsigned char mode) :
#ifdef ARDUINO_AVR_NANO
                                                                ioAdd{FindIOAddress(PinNum)},
#endif
                                                                PinNum{FindPinNumfunc(PinNum)}

    {
        if (PinNum != 0)
        {
            this->Low();
            pinMode(PinNum, mode);
        }
    }

    void Set(bool val) const;
    void High() const;
    void Low() const;
    unsigned char Read() const;
};

// Analog Pin Control
class AnPin
{
protected:
    const unsigned char pin;
    unsigned int val;
    unsigned int MaxValue;

public:
    AnPin(const unsigned char pin) : pin{pin}
    {
        if (pin != 0)
        {
            pinMode(pin, OUTPUT);
        }
        MaxValue = 100;
    }

    void Set(unsigned int val);
    unsigned int Get() const;
    unsigned int SetMaxValue(unsigned int newMax = 0);
};

// Digital Pin Control
class PinOpenCol : public Pin
{
public:
    PinOpenCol(const unsigned char PinNum) : Pin(PinNum, INPUT)
    {
        if (PinNum != 0)
        {
            this->Low();
            pinMode(PinNum, OUTPUT_OPENDRAIN);
        }
    }

    void Set(bool val) const;
    void High() const;
    void Low() const;
};

class PinOpenColInv : public PinOpenCol
{
public:
    PinOpenColInv(const unsigned char PinNum) : PinOpenCol(PinNum) {}

    void Set(bool val) const;
    void High() const;
    void Low() const;
};

// End Library Encapsulation
#endif