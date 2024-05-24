/*!
 * \file t3a33brg.cpp
 *
 * \mainpage T3A33BRG SPI Library
 *
 * \section intro_sec Introduction
 *
 * Example to control WS2801-based RGB LED Modules in a strand or strip
 *
 * \section author Author
 *
 * Written by PBeal
 * 
 * \section license License
 * GNU LESSER GENERAL PUBLIC LICENSE, V3.0
 */
#include "t3a33brg.h"

// Constructor for use with SPI instance:
SPI_T3A33BRG::SPI_T3A33BRG(uint16_t num, uint8_t mosi_pin, uint8_t sck_pin, spi_inst_t *spi)
{
    alloc(num);
    updatePins(mosi_pin, sck_pin);
    spi_interface = spi;
}

// Allocate 3 bytes per pixel, init to RGB 'off' state:
void SPI_T3A33BRG::alloc(uint16_t num)
{
    numLEDs = ((pixels = (uint8_t *)calloc(num, 3)) != NULL) ? num : 0;
}

// Release memory (as needed):
SPI_T3A33BRG::~SPI_T3A33BRG(void)
{
    if (pixels)
        free(pixels);
}

// Activate hard SPI:
void SPI_T3A33BRG::begin(void)
{
    // Initialize SPI port at 1 MHz
    spi_init(spi_interface, 1000 * 1000);

    // Set SPI format
    spi_set_format(spi_interface,          // SPI instance
                   8,             // Number of bits per transfer
                   (spi_cpol_t)1, // Polarity (CPOL)
                   (spi_cpha_t)1, // Phase (CPHA)
                   SPI_MSB_FIRST);

    // Initialize SPI pins
    gpio_set_function(sckpin, GPIO_FUNC_SPI);
    gpio_set_function(mosipin, GPIO_FUNC_SPI);

    // 32 consecutive 0’s denote the start of a command for an RGB LED
    startFrame[0] = 0x00;
    startFrame[1] = 0x00;
    startFrame[2] = 0x00;
    startFrame[3] = 0x00;

    // 32 consecutive 1’s denote the end of the current command for an RGB LED(End of Frame)
    endFrame[0] = 0xFF;
    endFrame[1] = 0xFF;
    endFrame[2] = 0xFF;
    endFrame[3] = 0xFF;
}

// Change pin assignments post-constructor, using arbitrary pins:
void SPI_T3A33BRG::updatePins(uint8_t mosi_pin, uint8_t sck_pin)
{
    gpio_set_dir(mosipin, GPIO_IN); // Restore data and clock pins to inputs
    gpio_set_dir(sckpin, GPIO_IN);

    // Initialize SPI pins
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);

    mosipin = mosi_pin;
    sckpin = sck_pin;
}

uint16_t SPI_T3A33BRG::numPixels(void) { return numLEDs; }

// Change strand length (see notes with empty constructor, above):
void SPI_T3A33BRG::updateLength(uint16_t num)
{
    if (pixels != NULL)
        free(pixels); // Free existing data (if any)
    // Allocate new data -- note: ALL PIXELS ARE CLEARED
    numLEDs = ((pixels = (uint8_t *)calloc(num, 3)) != NULL) ? num : 0;
}

void SPI_T3A33BRG::show(void)
{
    uint16_t i;
    const uint8_t *ledParamPtr = &ledParam;

    spi_write_blocking(spi_interface, startFrame, 4);
    // Write 24 bits per pixel:
    for (i = 0; i < numLEDs; i++)
    {
        uint8_t *redPtr = &pixels[i*3];
        uint8_t *greenPtr = &pixels[(i*3)+1];
        uint8_t *bluePtr = &pixels[(i*3)+2];

        spi_write_blocking(spi_interface, ledParamPtr, 1);
        spi_write_blocking(spi_interface, bluePtr, 1);
        spi_write_blocking(spi_interface, greenPtr, 1);
        spi_write_blocking(spi_interface, redPtr, 1);
    };
    spi_write_blocking(spi_interface, endFrame, 4);

    sleep_ms(1); // Data is latched by holding clock pin low for 1 millisecond
}

// Set pixel color from separate 8-bit R, G, B components:
void SPI_T3A33BRG::setPixelColor(uint16_t led, uint8_t red, uint8_t green,
                                 uint8_t blue)
{
    if (led < numLEDs)
    {
        uint8_t *p = &pixels[led * 3];
        *p++ = red;
        *p++ = green;
        *p++ = blue;
    }
}

// Set pixel color from 'packed' 32-bit RGB value:
void SPI_T3A33BRG::setPixelColor(uint16_t led, uint32_t color)
{
    if (led < numLEDs)
    {
        uint8_t *p = &pixels[led * 3];
        *p++ = color >> 16; // Red
        *p++ = color >> 8;  // Green
        *p++ = color;       // Blue
    }
}

// Clear all pixels
void SPI_T3A33BRG::clear()
{
    if (pixels != NULL)
    {
        memset(pixels, 0, numLEDs * 3);
    }
}

// Query color from previously-set pixel (returns packed 32-bit RGB value)
uint32_t SPI_T3A33BRG::getPixelColor(uint16_t led)
{
    if (led < numLEDs)
    {
        uint16_t ofs = led * 3;
        // To keep the show() loop as simple & fast as possible, the
        // internal color representation is native to different pixel
        // types.  For compatibility with existing code, 'packed' RGB
        // values passed in or out are always 0xRRGGBB order.
        return ((uint32_t)pixels[ofs] << 16) | ((uint16_t)pixels[ofs + 1] << 8) | ((uint16_t)pixels[ofs + 2]);
    }

    return 0; // Pixel # is out of bounds
}