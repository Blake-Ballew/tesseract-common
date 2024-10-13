#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32DMASPISlave.h>
#include <ESP32DMASPIMaster.h>

namespace TesseractCommon
{
    // Functions that take in a data buffer, a starting bit offset, a number of bits, and writes the value to the reference

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint8_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
        if (bitOffset >= 8) return;

        uint8_t returnVal = 0;
        uint8_t byteOffset = bitOffset >> 3;
        size_t bitOffsetInByte = (byteOffset << 3) - bitOffset;

        returnVal |= (data[byteOffset] >> bitOffsetInByte);

        if (bitOffsetInByte > 0)
        {
            uint8_t mask = 0xFF >> bitOffsetInByte;
            returnVal |= ((data[byteOffset + 1] & mask) << bitOffsetInByte);
        }

        // outVal = returnVal & (uint8_t)(0xFF << (8 - numBits));
    }

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint16_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
    }

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint32_t& outVal)
    {
        if (((bitOffset + numBits) << 3) > dataLen) return;
    }

    #pragma region WiFi

    const char *HostName = "Tesseract-Bridge";
    const IPAddress IpAddress(10, 0, 0, 69);
    const int ServerPort = 420;
    const int maxClients = 1;

    WiFiServer *Server = nullptr;
    WiFiClient Client;
    WiFiUDP UdpConnection;

    void EstablishWiFiConnection(
        wifi_mode_t mode = WIFI_MODE_STA,
        const char * hostname = HostName
        )
    {
        WiFi.mode(WIFI_STA);
        WiFi.setHostname(hostname);
    }

    void EstablishWiFiServer(
        IPAddress addr = IpAddress,
        int port = ServerPort
    )
    {
        UdpConnection.begin(addr, port);
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