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
unsigned char calcPWM(int val, int MaxVal)
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

//* *************
//*Pin
//* ******
void Pin::Set(bool val)
{
    val ? High() : Low();
}

void Pin::High()
{
#ifdef NonNano
    digitalWriteFast(this->PinNum, HIGH);
#else
    _SFR_IO8(this->ioAdd + 2) |= this->PinNum;
#endif
}

void Pin::Low()
{
#ifdef NonNano 
   digitalWriteFast(this->PinNum, LOW);
#else
    _SFR_IO8(this->ioAdd + 2) &= ~this->PinNum;
#endif
}

unsigned char Pin::Read()
{
#ifdef NonNano
    return digitalReadFast(this->PinNum);
#else
    return _SFR_IO8(this->ioAdd) & this->PinNum;
#endif
}

//* *************
//*AnPin
//* ******
void AnPin::Set(byte val)
{
    if (this->val != val)
    {
        this->val = val;
        analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
    }
}

int AnPin::Get()
{
    return val;
}

int AnPin::SetMaxValue(int newMax)
{
    if (newMax != 0 && MaxValue != newMax)
    {
        this->MaxValue = newMax;
        analogWrite(this->pin, calcPWM(this->val, this->MaxValue));
    }
    return this->MaxValue;
}