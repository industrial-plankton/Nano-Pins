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

#include <Nano-Pins.h>

// Calculate PWM value from value, passed as int to avoid overflow
unsigned int calcPWM(const unsigned int val, const unsigned int MaxVal)
{
#ifdef ARDUINO_AVR_NANO
    const uint16_t MaxAlalogOutut = 255;
#else
    const uint16_t MaxAlalogOutut = 32767; // TODO make this based on the analog resolution so that it can be changed from 15 bit without breaking things
#endif
    if (val >= MaxVal)
    {
        return MaxAlalogOutut;
    }
    else
    {
        // if inverted return (MaxAlalogOutut-(int)MaxAlalogOutut*val/MaxVal);
        return ((uint32_t)(MaxAlalogOutut * val) / MaxVal);
    }
}

unsigned char FindPinNumfunc(unsigned char pinnum)
{
#ifdef ARDUINO_AVR_NANO
    if (14 <= pinnum && pinnum <= 19) // A0-A5
    {
        pinnum -= 14;
        return _BV(pinnum);
    }
    else if (8 <= pinnum && pinnum <= 13) // D8-D13
    {
        pinnum -= 8;
        return _BV(pinnum);
    }
    else if (pinnum == 0) // dont allow pin 0
    {
        return 0;
    }
    else
    { // D0-D7
        return _BV(pinnum);
    }
#else
    return pinnum;
#endif
}

unsigned char FindIOAddress(unsigned char pinnum)
{
    if (14 <= pinnum && pinnum <= 19) // A0-A5
    {
        return 0x06;
    }
    else if (8 <= pinnum && pinnum <= 13) // D8-D13
    {
        return 0x03;
    }
    else if (pinnum == 0) // dont allow pin 0
    {
        return 0;
    }
    else
    { // D0-D7
        return 0x09;
    }
}

//* *************
//*Pin
//* ******
void Pin::Set(const bool val) const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
        val ? High() : Low();
    }
}

void Pin::High() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        _SFR_IO8(this->ioAdd + 2) |= this->PinNum;
#elif defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSYLC)
        digitalWriteFast(this->PinNum, HIGH);
#else
        digitalWrite(this->PinNum, HIGH);
#endif
    }
}

void Pin::Low() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        _SFR_IO8(this->ioAdd + 2) &= ~this->PinNum;
#elif defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSYLC)
        digitalWriteFast(this->PinNum, LOW);
#else
        digitalWrite(this->PinNum, LOW);
#endif
    }
}

unsigned char Pin::Read() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        return _SFR_IO8(this->ioAdd) & this->PinNum;
#elif defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSYLC)
        return digitalReadFast(this->PinNum);
#else
        return digitalRead(this->PinNum);
#endif
    }
    else
    {
        return 0;
    }
}

//* *************
//*AnPin
//* ******
void AnPin::Set(const unsigned int val)
{
    if (this->val != val)
    {
        this->val = val;
        if (this->pin != 0)
        {
            analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
        }
    }
}

void AnPin::Set(const unsigned int val, const unsigned int newMax)
{
    if (newMax != 0 && (MaxValue != newMax || this->val != val))
    {
        this->val = val;
        this->MaxValue = newMax;
        if (this->pin != 0)
        {
            analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
        }
    }
}

void AnPin::ConstSet(const unsigned int val) const
{
    if (this->pin != 0)
    {
        analogWrite(this->pin, calcPWM(val, this->MaxValue));
    }
}

unsigned int AnPin::Get() const
{
    return val;
}

unsigned int AnPin::SetMaxValue(const unsigned int newMax)
{
    if (newMax != 0 && MaxValue != newMax)
    {
        this->MaxValue = newMax;
        if (this->pin != 0)
        {
            analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
        }
    }
    return this->MaxValue;
}

#if defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSYLC)
//* *************
//*Pin Open Col/drain
//* ******
void PinOpenCol::Set(const bool val) const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
        val ? High() : Low();
    }
}

void PinOpenCol::High() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        // _SFR_IO8(this->ioAdd + 2) |= this->PinNum;
#else
        pinMode(this->PinNum, INPUT);
#endif
    }
}

void PinOpenCol::Low() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        // _SFR_IO8(this->ioAdd + 2) &= ~this->PinNum;
#else
        pinMode(this->PinNum, OUTPUT_OPENDRAIN);
#endif
    }
}

void PinOpenColInv::Set(const bool val) const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
        val ? High() : Low();
    }
}

void PinOpenColInv::High() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        // _SFR_IO8(this->ioAdd + 2) |= this->PinNum;
#else
        pinMode(this->PinNum, OUTPUT_OPENDRAIN);
#endif
    }
}

void PinOpenColInv::Low() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        // _SFR_IO8(this->ioAdd + 2) &= ~this->PinNum;
#else
        pinMode(this->PinNum, INPUT);
#endif
    }
}

#endif