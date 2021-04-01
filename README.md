# Nano-Pins

Simple Class to efficently control the pins of arduino nano, bypassing the inefficent digitalWrite() function, also allows reading from OUTPUT pins.

Ex:
```C++
// Initialize class, pin 2 as output
Pin pinname(2, OUTPUT);

pinname.Low(); // Set pin low
pinname.High(); // Set pin high
pinname.Read(); // check pin value

```

