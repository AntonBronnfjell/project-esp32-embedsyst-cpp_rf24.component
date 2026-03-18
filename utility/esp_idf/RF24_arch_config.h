/*
 * RF24 arch config for ESP-IDF
 */
#ifndef RF24_UTILITY_ESP_IDF_RF24_ARCH_CONFIG_H_
#define RF24_UTILITY_ESP_IDF_RF24_ARCH_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "spi.h"
#include "gpio.h"
#include "compatibility.h"
#include "interrupt.h"

#define _BV(x) (1 << (x))
/* When RF24_SPI_PTR is set we use _spi->, not _SPI; default set in _init_obj via rf24_default_spi_ref(). */
#define _SPI SPI

#ifdef RF24_DEBUG
    #define IF_RF24_DEBUG(x) ({ x; })
#else
    #define IF_RF24_DEBUG(x)
#endif

typedef uint16_t prog_uint16_t;
#define PSTR(x)  (x)
#define printf_P printf
#define strlen_P strlen
#define PROGMEM
#define pgm_read_word(p) (*(const unsigned short*)(p))
#define PRIPSTR "%s"
#define pgm_read_byte(p) (*(const unsigned char*)(p))
#define pgm_read_ptr(p)  (*(void* const*)(p))

#define LOW                      GPIO::OUTPUT_LOW
#define HIGH                     GPIO::OUTPUT_HIGH
#define INPUT                    GPIO::DIRECTION_IN
#define OUTPUT                   GPIO::DIRECTION_OUT
#define digitalWrite(pin, value) GPIO::write(pin, value)
#define pinMode(pin, direction)  GPIO::open(pin, direction)
#define delay(millisec)          __msleep(millisec)
#define delayMicroseconds(usec)  __usleep(usec)
#define millis()                 __millis()

#endif // RF24_UTILITY_ESP_IDF_RF24_ARCH_CONFIG_H_
