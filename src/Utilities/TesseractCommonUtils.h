#pragma once

#include <Arduino.h>

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
}