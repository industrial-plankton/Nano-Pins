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
unsigned char calcPWM(const unsigned int val, const unsigned int MaxVal)
{
    if (val >= MaxVal)
    {
        return 255;
    }
    else
    {
        // if inverted return (255-(int)255*val/MaxVal);
        return ((255 * val) / MaxVal);
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
#else
        digitalWriteFast(this->PinNum, HIGH);
#endif
    }
}

void Pin::Low() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        _SFR_IO8(this->ioAdd + 2) &= ~this->PinNum;
#else
        digitalWriteFast(this->PinNum, LOW);
#endif
    }
}

unsigned char Pin::Read() const
{
    if (this->PinNum != 0) // dont allow pin 0
    {
#ifdef ARDUINO_AVR_NANO
        return _SFR_IO8(this->ioAdd) & this->PinNum;
#else
        return digitalReadFast(this->PinNum);
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
        // digitalWrite(this->PinNum, LOW);
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
        // digitalWrite(this->PinNum, LOW);
        pinMode(this->PinNum, OUTPUT_OPENDRAIN);
#endif
    }
}