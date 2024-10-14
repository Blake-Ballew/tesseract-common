#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32DMASPISlave.h>
#include <ESP32DMASPIMaster.h>

namespace TesseractCommon
{

    #pragma region Bit Packing

    // Functions that take in a data buffer, a starting bit offset, a number of bits, and writes the value to the reference

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint8_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(uint8_t) << 3) return;

        uint8_t outValBitIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));

            auto andMask = GetLsbAndMask(bitsForThisByte);
            andMask <<= bitOffsetInByte;
            uint8_t bitsToAdd = (data[byteOffset] & andMask) >> bitOffsetInByte;

            outVal |= (bitsToAdd << outValBitIdx);
            outValBitIdx += bitsForThisByte;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
        }
    }

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint16_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(uint16_t) << 3) return;

        uint8_t outValBitIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));

            auto andMask = GetLsbAndMask(bitsForThisByte);
            andMask <<= bitOffsetInByte;
            uint8_t bitsToAdd = (data[byteOffset] & andMask) >> bitOffsetInByte;

            outVal |= (bitsToAdd << outValBitIdx);
            outValBitIdx += bitsForThisByte;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
        }
    }

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint32_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(uint32_t) << 3) return;

        uint8_t outValBitIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));

            auto andMask = GetLsbAndMask(bitsForThisByte);
            andMask <<= bitOffsetInByte;
            uint8_t bitsToAdd = (data[byteOffset] & andMask) >> bitOffsetInByte;

            outVal |= (bitsToAdd << outValBitIdx);
            outValBitIdx += bitsForThisByte;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
        }    }

    void SetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint8_t val)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(val) << 3) return;

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));
            auto andMask = GetLsbAndMask(bitsForThisByte);
            uint8_t bitsToAdd = (val & andMask) << bitOffsetInByte;
            data[byteOffset] |= bitsToAdd;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
            // currValIdx += bitsForThisByte;

            val >>= bitsForThisByte;
        }
    }

    void SetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint16_t val)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(val) << 3) return;

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));
            auto andMask = GetLsbAndMask(bitsForThisByte);
            uint8_t bitsToAdd = (val & andMask) << bitOffsetInByte;
            data[byteOffset] |= bitsToAdd;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
            // currValIdx += bitsForThisByte;

            val >>= bitsForThisByte;
        }
    }

    void SetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint32_t val)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (numBits > sizeof(val) << 3) return;

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));
            auto andMask = GetLsbAndMask(bitsForThisByte);
            uint8_t bitsToAdd = (val & andMask) << bitOffsetInByte;
            data[byteOffset] |= bitsToAdd;

            bitOffset += bitsForThisByte;
            numBits -= bitsForThisByte;
            // currValIdx += bitsForThisByte;

            val >>= bitsForThisByte;
        }
    }

    uint8_t GetLsbAndMask(uint8_t numBits)
    {
        uint16_t mask = (1 << numBits) - 1;
        return (uint8_t)mask;
    }

    #pragma endregion

    #pragma region WiFi

    const char *SSID = "Tesseract";
    const char *Password = "Tesseract";
    const char *HostName = "Tesseract-Bridge";
    const IPAddress IpAddress(10, 0, 0, 69);
    const int ConnectionPort = 420;
    const size_t ConnectionTimeout = 5000;


    WiFiServer *Server = nullptr;
    WiFiClient Client;
    WiFiUDP UdpConnection;

    void EstablishWiFiConnection(
        wifi_mode_t mode = WIFI_MODE_STA,
        const char * hostname = HostName,
        size_t timeOutms = ConnectionTimeout,
        const char * ssid = SSID,
        const char * password = Password
        )
    {
        WiFi.mode(WIFI_STA);
        WiFi.setHostname(hostname);

        if (mode == WIFI_MODE_STA)
        {
            WiFi.begin(ssid, password);
        }
        else if (mode == WIFI_MODE_AP)
        {
            WiFi.softAP(ssid, password);
        }

        WiFi.onEvent(WiFiEvent);

        if (timeOutms > 0)
        {
            unsigned long startTime = millis();
            while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeOutms)
            {
                delay(100);
            }
        }
    }

    void EstablishUdpStream(
        IPAddress addr = IpAddress,
        int port = ConnectionPort
    )
    {
        UdpConnection.begin(addr, port);
    }

    void WiFiEvent(WiFiEvent_t event)
    {
        switch (event)
        {
            
        }
    }

    // void CheckForWiFiClient()
    // {
    //     if (Server->hasClient())
    //     {
    //         if (Client.connected())
    //         {
    //             Client.stop();
    //         }

    //         Client = Server->available();
    //     }
    // }

    #pragma endregion

    #pragma region SPI Connection

    const size_t SPI_BUFFER_SIZE = 1024;
    const size_t SPI_BUFFER_PADDING = 4;
    const size_t SPI_QUEUE_SIZE = 1;
    const size_t SPI_FREQUENCY = 1000000;

    uint8_t *SpiReceiveBuffer = nullptr;
    uint8_t *SpiSendBuffer = nullptr;

    ESP32DMASPI::Master master;
    ESP32DMASPI::Slave slave;

    bool SpiInitialized = false;
    bool SpiMaster = false;

    void EstablishSPIMaster(
        size_t bufferSize = SPI_BUFFER_SIZE,
        size_t queueSize = SPI_QUEUE_SIZE,
        size_t spiMode = SPI_MODE0,
        size_t frequency = SPI_FREQUENCY
    )
    {
        SpiMaster = true;

        SpiSendBuffer = master.allocDMABuffer(bufferSize + SPI_BUFFER_PADDING);
        SpiReceiveBuffer = master.allocDMABuffer(bufferSize + SPI_BUFFER_PADDING);

        master.setDataMode(spiMode);
        master.setMaxTransferSize(bufferSize);
        master.setQueueSize(queueSize);
        master.setFrequency(frequency);
        master.begin();

        SpiInitialized = true;
    }


    void EstablishSPISlave(
        size_t bufferSize = SPI_BUFFER_SIZE,
        size_t queueSize = SPI_QUEUE_SIZE,
        size_t spiMode = SPI_MODE0
    )
    {
        SpiMaster = false;

        SpiSendBuffer = slave.allocDMABuffer(bufferSize + SPI_BUFFER_PADDING);
        SpiReceiveBuffer = slave.allocDMABuffer(bufferSize + SPI_BUFFER_PADDING);

        slave.setDataMode(spiMode);
        slave.setMaxTransferSize(bufferSize);
        slave.setQueueSize(queueSize);
        slave.begin();

        SpiInitialized = true;
    }

    // Streams data from master to slave. The stream will be a udp connection
    void StreamDataToMasterBuffer(Stream &stream)
    {
        if (!SpiInitialized) return;

        auto bytesAvailable = stream.available();
        if (bytesAvailable < 1) return;
        
        if (bytesAvailable > 0)

        {
            memset(SpiSendBuffer, 0, SPI_BUFFER_SIZE + SPI_BUFFER_PADDING);
            stream.readBytes(SpiSendBuffer, bytesAvailable);
        }

        master.transfer(SpiSendBuffer, SpiReceiveBuffer, bytesAvailable);
    }

    #pragma endregion
}