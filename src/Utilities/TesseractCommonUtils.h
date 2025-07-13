#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>



#define MOSI_PIN 11
#define MISO_PIN 13
#define SCLK_PIN 12
#define CS_PIN 10

namespace TesseractCommon
{

    #pragma region Bit Packing

    // Functions that take in a data buffer, a starting bit offset, a number of bits, and writes the value to the reference

    uint8_t GetLsbAndMask(uint8_t numBits)
    {
        uint16_t mask = (1 << numBits) - 1;
        return (uint8_t)mask;
    }

    void GetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint8_t& outVal)
    {
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read outside of data buffer");
            return;
        }

        if (numBits > sizeof(uint8_t) << 3)
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read more than 8 bits");
            return;
        }

        uint8_t outValBitIdx = 0;
        outVal = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

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
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read outside of data buffer");
            return;
        }

        if (numBits > sizeof(uint16_t) << 3)
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read more than 16 bits");
            return;
        }

        uint8_t outValBitIdx = 0;
        outVal = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

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
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read outside of data buffer");
            return;
        }

        if (numBits > sizeof(uint32_t) << 3)
        {
            // Serial.println("ERROR: GetBitCompressedValue: Attempted to read more than 32 bits");
            return;
        }

        uint8_t outValBitIdx = 0;
        outVal = 0; 

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

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

    void SetBitCompressedValue(uint8_t* data, size_t dataLen, size_t bitOffset, uint8_t numBits, uint8_t val)
    {
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write outside of data buffer");
            return;
        }

        if (numBits > sizeof(val) << 3)
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write more than 8 bits");
            return;
        }

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

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
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write outside of data buffer");
            return;
        }

        if (numBits > sizeof(val) << 3)
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write more than 16 bits");
            return;
        }

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

            size_t bitsForThisByte = min(numBits, uint8_t(8 - bitOffsetInByte));
            Serial.printf("byteOffset: %d, bitOffsetInByte: %d, bitsForThisByte: %d\n", byteOffset, bitOffsetInByte, bitsForThisByte);
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
        if (((bitOffset + numBits)) > (dataLen << 3))
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write outside of data buffer");
            return;
        }

        if (numBits > sizeof(val) << 3)
        {
            // Serial.println("ERROR: SetBitCompressedValue: Attempted to write more than 32 bits");
            return;
        }

        // Either track progress with index, or subtract from numBits and shift the val out
        // uint8_t currValIdx = 0;

        while (numBits > 0)
        {
            size_t byteOffset = bitOffset >> 3;
            uint8_t bitOffsetInByte = bitOffset - (byteOffset << 3);

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

    void WiFiEvent(WiFiEvent_t event)
    {
        switch (event)
        {
            
        }
    }

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
            delay(100); // Allow time for AP to start
            WiFi.softAPConfig(IpAddress, IpAddress, IPAddress(255, 255, 255, 0));
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


}