#include <Wire.h>
#include <ADC128D818.h>

#define I2C_ADDRESS 0x1D // Change this to the address of your ADC.

ADC128 adc(I2C_ADDRESS);

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    adc.reset(); // Make sure all the default values are used.

    adc.set_conversion_rate(CONTINUOUS);
    adc.start();
}

void loop()
{
    for (uint8_t i = 0; i < 8; i++) // loop over all the enabled channels.
    {
        Serial.println(adc.read_ADC_channel(i)); // Print the measured values to the Serial port.
    }

    delay(100); // Give the ADC some time to finish its measurements. This value can be calculated by the formula in the datasheet.
}