#include "ADC128D818.h"

// Public functions

ADC128::ADC128(uint8_t address, TwoWire *theWire)
{
    _i2caddr = address;
    _i2c = theWire;
}

bool ADC128::available(void)
{
    _i2c->beginTransmission(_i2caddr);
    if (_i2c->endTransmission() != 0)
    {
        return false;
    }

    return true;
}

bool ADC128::start(void)
{
    _configuration |= 1;

    return write(CONFIGURATION, _configuration);
}

bool ADC128::stop(void)
{
    _configuration &= ~1;

    return write(CONFIGURATION, _configuration);
}

bool ADC128::reset(void)
{
    _configuration |= (1 << 7);

    return write(CONFIGURATION, _configuration);
}

bool ADC128::write_configuration(ADC128_configuration configuration)
{
    uint8_t data = (configuration.start) + (configuration.interupt_enable << 1) + (configuration.clear_interrupt << 3) + (configuration.clear_initialization << 7);

    _configuration = data;

    return write(CONFIGURATION, data);
}

bool ADC128::read_configuration(ADC128_configuration *configuration)
{
    uint8_t reply;

    if (!read(CONFIGURATION, &reply, 1, 0))
    {
        return false;
    }

    _configuration = reply;

    configuration->start = reply & 0x01;
    configuration->interupt_enable = (reply >> 1) & 0x01;
    configuration->clear_interrupt = (reply >> 3) & 0x01;
    configuration->clear_initialization = (reply >> 7) & 0x01;

    return true;
}

bool ADC128::read_interrupt_state(ADC128_channels *interrupts)
{
    uint8_t reply;

    if (!read(INTERRUPT_STATUS, &reply, 1, 0))
    {
        return false;
    }

    interrupts->from_byte(reply);

    return true;
}

bool ADC128::write_interrupt_mask(ADC128_channels masks)
{
    return write(INTERRUPT_MASK, masks.to_byte());
}

bool ADC128::read_interrupt_mask(ADC128_channels *masks)
{
    uint8_t reply;

    if (!read(INTERRUPT_MASK, &reply, 1, 0))
    {
        return false;
    }

    masks->from_byte(reply);

    return true;
}

// set_conversion_rate sets the conversion rate to either
// true: continuous conversion mode
// false: low power conversion mode
// As the setting can only be done when the device is in shutdown mode we will first stop the device, edit the conversion rate and start it again.
bool ADC128::set_conversion_rate(bool mode)
{
    if (!ADC128::stop())
    {
        return false;
    }

    if (!write(CONVERSION_RATE, mode))
    {
        return false;
    }

    return ADC128::start();
}

bool ADC128::write_disabled_channels(ADC128_channels disabled)
{
    return write(CHANNEL_DISABLE, disabled.to_byte());
}

bool ADC128::read_disabled_channels(ADC128_channels *disabled)
{
    uint8_t reply;

    if (!read(CHANNEL_DISABLE, &reply, 1, 0))
    {
        return false;
    }

    disabled->from_byte(reply);

    return true;
}

bool ADC128::initiate_single_conversion(void)
{
    return write(ONE_SHOT, true);
}

bool ADC128::initiate_deep_shutdown(void)
{
    ADC128::stop();

    return write(DEEP_SHUTDOWN, true);
}

bool ADC128::write_advanced_configuration(ADC128_advanced_configuration configuration)
{
    uint8_t data = (configuration.external_reference_enable) + (configuration.selected_mode << 1);

    return write(ADVANCED_CONFIGURATION, data);
}

bool ADC128::read_advanced_configuration(ADC128_advanced_configuration *configuration)
{
    uint8_t reply;

    if (!read(ADVANCED_CONFIGURATION, &reply, 1, 0))
    {
        return false;
    }

    configuration->external_reference_enable = reply & 0x01;
    configuration->selected_mode = (reply & 0x06) >> 1;

    return true;
}

// read_busy_status returns true if the device is busy with either converting or starting up.
bool ADC128::read_busy_status(void)
{
    uint8_t reply;

    read(BUSY_STATUS, &reply, 1, 0);

    if ((reply & 0x01) == 1 || ((reply >> 1) & 0x1) == 1))
        {
            return true;
        }

    return false;
}

uint16_t ADC128::read_ADC_channel(uint8_t channel)
{
    if (channel > 7)
    {
        return false;
    }

    uint8_t reply[2];

    read(CHANNEL_READING_START + channel, reply, 2, 0);

    return (uint16_t)(reply[0] << 8) + reply[1]; // Assuming it is MSB first, datasheet is unclear on the matter. Will need to test.
}

// set_channel_limits sets the upper and lower channel limits.
bool ADC128::set_channel_limits(uint8_t channel, int8_t high_limit, int8_t low_limit)
{
    if (channel > 7)
    {
        return false;
    }

    if (!write(CHANNEL_LIMITS_START + (channel * 2), high_limit))
    {
        return false;
    }

    if (!write(CHANNEL_LIMITS_START + (channel * 2) + 1, low_limit))
    {
        return false;
    }
}

uint8_t ADC128::read_manufacturer_id(void)
{
    uint8_t reply;

    read(MANUFACTURER_ID, &reply, 1, 0);

    return reply;
}

uint8_t ADC128::read_revision_id(void)
{
    uint8_t reply;

    read(REVISION_ID, &reply, 1, 0);

    return reply;
}

// Private functions

bool ADC128::write(uint8_t command, uint8_t data)
{
    _i2c->beginTransmission(_i2caddr);
    _i2c->write(command);
    _i2c->write(data);
    if (_i2c->endTransmission() != 0)
    {
        return false;
    }

    return true;
}

bool ADC128::read(uint8_t command, uint8_t *readdata, uint8_t readlen, uint8_t _delay)
{
    _i2c->beginTransmission(_i2caddr);
    _i2c->write(command);
    _i2c->endTransmission();

    delay(_delay);

    _i2c->requestFrom(_i2caddr, readlen);

    for (int i = 0; i < readlen; i++)
    {
        readdata[i] = _i2c->read();
    }

    return true;
}