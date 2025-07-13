#pragma once

#include "../include/ESP32DMASPIMaster.h"

namespace TesseractCommon
{
    class SpiMasterHelper
    {
    public:
        SpiMasterHelper() {}

        SpiMasterHelper(int csPin, int sckPin = 12, int misoPin = 13, int mosiPin = 11, uint8_t spiBus = HSPI,
            size_t bufferSize = 1024, size_t queueSize = 1, size_t spiMode = SPI_MODE0, size_t frequency = 1000000)
            : _csPin(csPin), _sckPin(sckPin), _misoPin(misoPin), _mosiPin(mosiPin), _spiBus(spiBus),
            _bufferSize(bufferSize), _queueSize(queueSize), _spiMode(spiMode), _frequency(frequency) {}

        ~SpiMasterHelper() {}

        bool begin()
        {
            if (_initialized) return true;

            pinMode(_csPin, OUTPUT);
            digitalWrite(_csPin, HIGH);

            SpiSendBuffer = _master.allocDMABuffer(_bufferSize + SPI_BUFFER_PADDING);
            SpiReceiveBuffer = _master.allocDMABuffer(_bufferSize + SPI_BUFFER_PADDING);

            _master.setDataMode(_spiMode);
            _master.setMaxTransferSize(_bufferSize);
            _master.setQueueSize(_queueSize);
            _master.setFrequency(_frequency);
            _initialized = _master.begin(_spiBus, _sckPin, _misoPin, _mosiPin, _csPin);

            return _initialized && SpiSendBuffer != nullptr && SpiReceiveBuffer != nullptr;
        }

        void SendDataToSlave(uint8_t *data, size_t length, size_t timeoutMS = 500)
        {
            if (!_initialized) {
                Serial.println("SPI Master not initialized!");
                return;
            }

            if (length > _bufferSize) {
                Serial.println("Data length exceeds buffer size!");
                return;
            }

            memset(SpiSendBuffer, 0, _bufferSize + SPI_BUFFER_PADDING);
            memcpy(SpiSendBuffer, data, length);

            digitalWrite(_csPin, LOW);
            _master.transfer(SpiSendBuffer, SpiReceiveBuffer, length, timeoutMS);
            digitalWrite(_csPin, HIGH);
        }

    private:
        ESP32DMASPIMaster::Master _master;
        bool _initialized = false;

        int _csPin = 10; // Default CS pin
        int _sckPin = 12; // Default SCK pin
        int _misoPin = 13; // Default MISO pin
        int _mosiPin = 11; // Default MOSI pin

        uint8_t _spiBus = HSPI; // Default SPI bus

        size_t _bufferSize = 1024; // Default buffer size
        size_t _queueSize = 1; // Default queue size
        size_t _spiMode = SPI_MODE0;
        size_t _frequency = 1000000; // Default frequency

        uint8_t *SpiReceiveBuffer = nullptr;
        uint8_t *SpiSendBuffer = nullptr;

        const size_t SPI_BUFFER_PADDING = 4; // Padding for DMA buffer
    };
};