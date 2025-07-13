#pragma once

#include "../include/ESP32DMASPISlave.h"

namespace TesseractCommon
{
    class SpiSlaveHelper
    {
    public:
    SpiSlaveHelper() {}

        SpiSlaveHelper(int csPin, int sckPin = 12, int misoPin = 13, int mosiPin = 11, uint8_t spiBus = HSPI,
            size_t bufferSize = 1024, size_t queueSize = 1, size_t spiMode = SPI_MODE0)
            : _csPin(csPin), _sckPin(sckPin), _misoPin(misoPin), _mosiPin(mosiPin), _spiBus(spiBus),
            _bufferSize(bufferSize), _queueSize(queueSize), _spiMode(spiMode) {}

        ~SpiSlaveHelper() {}

        bool begin()
        {
            if (_initialized) return true;

            pinMode(_csPin, INPUT);

            SpiSendBuffer = _slave.allocDMABuffer(_bufferSize + SPI_BUFFER_PADDING);
            SpiReceiveBuffer = _slave.allocDMABuffer(_bufferSize + SPI_BUFFER_PADDING);

            _slave.setDataMode(_spiMode);
            _slave.setMaxTransferSize(_bufferSize);
            _slave.setQueueSize(_queueSize);
            _initialized = _slave.begin(_spiBus, _sckPin, _misoPin, _mosiPin, _csPin);

            return _initialized && SpiSendBuffer != nullptr && SpiReceiveBuffer != nullptr;
        }

        size_t ReceiveDataFromMaster(uint8_t *&data)
        {
            if (!_initialized) {
                Serial.println("SPI Slave not initialized!");
                return 0;
            }

            memset(SpiReceiveBuffer, 0, _bufferSize + SPI_BUFFER_PADDING);
            size_t receivedLength = _slave.transfer(SpiSendBuffer, SpiReceiveBuffer, _bufferSize, 500);
            data = SpiReceiveBuffer;
            return receivedLength;
        }

    private:
        ESP32DMASPISlave::Slave _slave;
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