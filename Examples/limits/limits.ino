#include <Wire.h>
#include <ADC128D818.h>

#define I2C_ADDRESS 0x1D // Change this to the address of your ADC.
#define INTERRUPT_PIN 2

ADC128 adc(I2C_ADDRESS);

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    adc.reset(); // Make sure all the default values are used.

    // Set the minimum and maximum levels of channel 0.
    // When the measured value is respectivily below or above these values the interrupt pin will be triggered.
    set_channel_limits(0, 100, 3000);

    ADC128_channels interrupt;
    interrupt.channel_0 = true; // Enable the interrupt on channel 0.

    adc.write_interrupt_mask(interrupt); // Write the interrupt configuration to the ADC.

    ADC128_configuration configuration;
    configuration.interupt_enable = 1; // Enable the interrupt pin.
    
    adc.write_configuration(configuration); // Write the configuration to the ADC.

    adc.set_conversion_rate(CONTINUOUS); // Set the conversion rate to continuous operation.
    adc.start(); // Start the ADC measurements.
}

void loop()
{
    if (!digitalRead(0)) // If the interrupt is low it is triggered
    {
        ADC128_channels interrupt;
        adc.read_interrupt_state(&interrupt);

        if (interrupt.channel_0)
        {
            // If the channel has interrupted do something with this exceedance.
        }
    }
}