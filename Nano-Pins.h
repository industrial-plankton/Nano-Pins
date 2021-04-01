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