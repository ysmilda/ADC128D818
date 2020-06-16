#include <Wire.h>
#include <ADC128D818.h>

#define I2C_ADDRESS 0x1D // Change this to the address of your ADC.

ADC128 adc(I2C_ADDRESS);

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    adc.reset(); // Make sure all the default values are used.

    ADC128_channels disabled_channels;
    disabled_channels.channel_0 = true; // Disable the channels that won't be used.
    disabled_channels.channel_7 = true;

    adc.write_disabled_channels(disabled_channels); // Write the channels that should be disabled to the ADC.

    // After setting the disabled channels put the ADC into deep shutdown mode. Current draw should now be 10uA.
    adc.initiate_deep_shutdown();
}

void loop()
{
    adc.initiate_single_conversion(); // Start a one of measurement, after this the ADC will return to it's previous state.

    delay(100); // Give the ADC some time to finish its measurements. This value can be calculated by the formula in the datasheet.

    for (uint8_t i = 1; i < 7; i++) // loop over all the enabled channels.
    {
        Serial.println(adc.read_ADC_channel(i)); // Print the measured values to the Serial port.
    }

    // Do something else for some time.
}