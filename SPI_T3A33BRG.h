#pragma once
/*!
 * \file SPI_T3A33BRG.h
 */

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <cstdlib>
#include <string.h>

class SPI_T3A33BRG
{

public:
    /*!
     * \brief Constructor for use with SPI instance
     *
     * \param num How many LEDs there are
     * \param mosi_pin Master Out Slave In pin
     * \param sck_pin Clock pin
     * \param spi SPI instance specifier, either \ref spi0 or \ref spi1
     */
    SPI_T3A33BRG(uint16_t num, uint8_t mosi_pin, uint8_t sck_pin, spi_inst_t *spi);

    /*!
     * \brief Release memory (as needed):
     */
    ~SPI_T3A33BRG();

    /*!
     * \brief Activate SPI
     */
    void begin(void);

    /*!
     * \brief Shows the pixels
     */
    void show(void);

    /*!
     * \brief Set pixel color from separate 8-bit R, G, B components
     *
     * \param led Pixel to set
     * \param red Red value
     * \param green Green value
     * \param blue Blue value
     */
    void setPixelColor(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);

    /*!
     * \brief Set pixel color from 'packed' 32-bit RGB value
     *
     * \param led Pixel to set
     * \param color packed 32-bit RGB value to set the pixel
     */
    void setPixelColor(uint16_t led, uint32_t color);

    /*!
     * \brief Clear the entire string
     */
    void clear();

    /*!
     * \brief Change pin assignment post-constructor, using arbitrary pins
     *
     * \param mosi_pin Data pin
     * \param sck_pin Clock pin
     */
    void updatePins(uint8_t mosi_pin, uint8_t sck_pin);

    /*!
     * \brief Change strand length
     *
     * \param numLEDs Length to change to
     */
    void updateLength(uint16_t num);
    
    /*!
     * \brief Returns the number of pixels currently connected
     *
     * \return Returns the number of connected pixels
     */
    uint16_t numPixels(void);
    
    /*!
     * \brief Query color from previously-set pixel
     * \param n Pixel to query
     * \return Returns packed 32-bit RGB value
     */
    uint32_t getPixelColor(uint16_t n);

private:
    uint16_t numLEDs;
    uint8_t *pixels,     // Holds color values for each LED (3 bytes each)
        sckpin, mosipin; // Clock & data pin numbers

    uint8_t startFrame[4], endFrame[4];
    uint8_t ledParam = 0xFF;

    // SPI instance specifier, either spi0 or spi1
    spi_inst_t *spi_interface;

    void alloc(uint16_t n);
};
