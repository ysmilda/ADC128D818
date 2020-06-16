# ADC128D818 8 channel ADC library

This library is made to interface with a Texas Instruments ADC128D818 ADC with the Arduino platform. The ADC communicates via the I2C bus and for this the library uses the Wire interface. Porting this to any other platform should be as easy as replacing Wire with your platform specific I2C implementation.

## Basic usage

The basic usage of this library can be as easy as:
```cpp
ADC128 adc(I2C_address);

adc.set_conversion_rate(CONTINUOUS);
adc.start();
adc.read_ADC_channel(x);
```

But this ADC supports a lot of different configurations. This readme will give a couple examples but for all the possible functions you should check the [datasheet](https://www.ti.com/lit/ds/symlink/adc128d818.pdf).

The library tries to approach the ADC in a structured and reusable manner. All the registers that have bitvalues for the different channels can be written to using the `ADC128_channels` struct. This contains a boolean for every channel and a `to_byte()` and `from_byte()` function for easy (un)marshalling. Both configuration registers have their own struct with the respective values. The usage of these is explained in limits and low_power_mode examples.
