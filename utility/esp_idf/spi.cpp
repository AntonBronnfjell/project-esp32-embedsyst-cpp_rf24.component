/**
 * SPI implementation for ESP-IDF
 */
#include "spi.h"
#include "esp_log.h"
#include <cstring>

static const char* TAG = "RF24_SPI";

SPI::SPI(spi_host_device_t host)
    : _host(host), _sck(-1), _miso(-1), _mosi(-1), _csn_pin(-1),
      _spi_speed(RF24_SPI_SPEED), _initialized(false), _handle(nullptr)
{
}

SPI::~SPI()
{
    if (_handle) {
        spi_bus_remove_device(_handle);
        _handle = nullptr;
    }
    _initialized = false;
}

void SPI::setPins(int sck, int miso, int mosi)
{
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
}

void SPI::begin(int sck, int miso, int mosi, int csn_pin, uint32_t spi_speed)
{
    if (_initialized) {
        return;
    }
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
    _csn_pin = csn_pin;
    _spi_speed = spi_speed;
    init();
    _initialized = true;
}

void SPI::begin()
{
    if (_initialized) {
        return;
    }
    if (_sck < 0 || _miso < 0 || _mosi < 0 || _csn_pin < 0) {
        return;
    }
    init();
    _initialized = true;
}

void SPI::init()
{
    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num = _mosi;
    bus_cfg.miso_io_num = _miso;
    bus_cfg.sclk_io_num = _sck;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 32;

    esp_err_t ret = spi_bus_initialize(_host, &bus_cfg, SPI_DMA_DISABLED);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(ret));
        return;
    }

    spi_device_interface_config_t dev_cfg = {};
    dev_cfg.clock_speed_hz = (int)_spi_speed;
    dev_cfg.mode = 0;
    dev_cfg.spics_io_num = _csn_pin;
    dev_cfg.queue_size = 1;

    ret = spi_bus_add_device(_host, &dev_cfg, &_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_add_device failed: %s", esp_err_to_name(ret));
        spi_bus_free(_host);
        return;
    }
}

uint8_t SPI::transfer(uint8_t tx)
{
    uint8_t rx = 0;
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &tx;
    t.rx_buffer = &rx;
    if (spi_device_polling_transmit(_handle, &t) != ESP_OK) {
        return 0;
    }
    return rx;
}

void SPI::transfernb(char* txBuf, char* rxBuf, uint32_t len)
{
    spi_transaction_t t = {};
    t.length = len * 8;
    t.tx_buffer = txBuf;
    t.rx_buffer = rxBuf;
    spi_device_polling_transmit(_handle, &t);
}

void SPI::transfern(char* buf, uint32_t len)
{
    transfernb(buf, buf, len);
}

SPI_t SPI(SPI2_HOST);

SPI_t& rf24_default_spi_ref()
{
    return SPI;
}
