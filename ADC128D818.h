#ifndef ADC818D818_H
#define ADC818D818_H

#include <Arduino.h>
#include <Wire.h>

typedef struct ADC128_configuration
{
    bool start = 0; // Starts the conversions.
    bool interupt_enable = 0;
    bool clear_interrupt = 0;
    bool clear_initialization = 0; // Reinitialize resets all the settings
};

typedef struct ADC128_advanced_configuration
{
    bool external_reference_enable = 0;
    uint8_t selected_mode = 0; // For the reference of the modes please check the datasheet.
};

typedef struct ADC128_channels // A reuseable struct for all the registers containing bit data about all the channels.
{
    bool channel_0 = 0;
    bool channel_1 = 0;
    bool channel_2 = 0;
    bool channel_3 = 0;
    bool channel_4 = 0;
    bool channel_5 = 0;
    bool channel_6 = 0;
    bool channel_7 = 0;

    uint8_t to_byte(void)
    {
        return (channel_0) + (channel_1 << 1) + (channel_2 << 2) + (channel_3 << 3) + (channel_4 << 4) + (channel_5 << 5) + (channel_6 << 6) + (channel_7 << 7);
    }

    void from_byte(uint8_t input)
    {
        channel_0 = input & 0x01;
        channel_1 = (input >> 1) & 0x01;
        channel_2 = (input >> 2) & 0x01;
        channel_3 = (input >> 3) & 0x01;
        channel_4 = (input >> 4) & 0x01;
        channel_5 = (input >> 5) & 0x01;
        channel_6 = (input >> 6) & 0x01;
        channel_7 = (input >> 7) & 0x01;
    }
};

enum conversion_rate
{
    LOW_POWER,
    CONTINUOUS
};

class ADC128
{
public:
    ADC128(uint8_t address, TwoWire *theWire = &Wire);

    bool available(void);
    bool start(void);
    bool stop(void);
    bool reset(void);

    bool write_configuration(ADC128_configuration configuration);
    bool read_configuration(ADC128_configuration *configuration);

    bool read_interrupt_state(ADC128_channels *interrupts);

    bool write_interrupt_mask(ADC128_channels masks);
    bool read_interrupt_mask(ADC128_channels *masks);

    bool set_conversion_rate(bool mode);

    bool write_disabled_channels(ADC128_channels disabled);
    bool read_disabled_channels(ADC128_channels *disabled);

    bool initiate_single_conversion(void);
    bool initiate_deep_shutdown(void);

    bool write_advanced_configuration(ADC128_advanced_configuration configuration);
    bool read_advanced_configuration(ADC128_advanced_configuration *configuration);

    bool read_busy_status(void);

    uint16_t read_ADC_channel(uint8_t channel);

    bool set_channel_limits(uint8_t channel, int8_t high_limit, int8_t low_limit);

    uint8_t read_manufacturer_id(void);

    uint8_t read_revision_id(void);

private:
    bool read(uint8_t command, uint8_t *readdata, uint8_t readlen, uint8_t _delay);
    bool write(uint8_t command, uint8_t data);

    uint8_t _configuration = 0x08;

    uint8_t _i2caddr;
    TwoWire *_i2c;

    enum ADC128_registers
    {
        CONFIGURATION = 0x00,
        INTERRUPT_STATUS = 0x01,
        INTERRUPT_MASK = 0x03,
        CONVERSION_RATE = 0x07,
        CHANNEL_DISABLE = 0x08,
        ONE_SHOT = 0x09,
        DEEP_SHUTDOWN = 0x0A,
        ADVANCED_CONFIGURATION = 0x0B,
        BUSY_STATUS = 0x0C,
        CHANNEL_READING_START = 0x20,
        CHANNEL_LIMITS_START = 0x2A,
        MANUFACTURER_ID = 0x3E,
        REVISION_ID = 0x3F
    };
};

#endif