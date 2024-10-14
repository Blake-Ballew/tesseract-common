#pragma once

#include <Arduino.h>
#include "TesseractCommonUtils.h"

namespace TesseractCommon
{
    // Opcodes for GPU commands. 6 bits
    namespace DrawCommandOpcode
    {
        const uint8_t OPCODE_SIZE_BITS = 6;

        const uint8_t CFG_ZLVL = 0x01; // SetStripZLevel
        const uint8_t CFG_OFS = 0x02;  // SetTimingOffset
        const uint8_t CFG_TSCL = 0x03; // SetTimingScale

        const uint8_t CTRL_CLR = 0x04; // ClearMemory
        const uint8_t CTRL_ZLQ = 0x05; // SetZLevelQuad
        const uint8_t CTRL_COLOR = 0x06; // SetPaletteColor

        const uint8_t DRW_ZORD = 0x07; // DrawZOrderPixels
        const uint8_t DRW_XY_PXL = 0x08; // DrawXYPixel
        const uint8_t DRW_XY_RECT = 0x09; // DrawRect
    }

    struct DrawCommand
    {
        uint8_t opcode; // 6 bits
    };

    struct SetStripZLevel
    {
        uint8_t zLevel; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 8, zLevel);

            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 8, zLevel);
            bitOffset += 8;
        }
    };

    struct SetTimingOffset
    {
        uint32_t offsetNanoseconds; // 24 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 24, offsetNanoseconds);

            bitOffset += 24;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 24, offsetNanoseconds);
            bitOffset += 24;
        }
    };

    struct SetTimingScale
    {
        uint32_t timeScaleFactor; // 24 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 24, timeScaleFactor);

            bitOffset += 24;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 24, timeScaleFactor);
            bitOffset += 24;
        }
    };

    struct ClearMemory
    {
        uint16_t padding; // 10 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 10, padding);
            bitOffset += 10;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 10, padding);
            bitOffset += 10;
        }
    };

    struct SetZLevel
    {
        uint8_t quadrant; // 2 bits
        uint8_t zLevel; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 2, quadrant);
            bitOffset += 2;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, zLevel);
            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 2, quadrant);
            bitOffset += 2;

            SetBitCompressedValue(data, dataLen, bitOffset, 8, zLevel);
            bitOffset += 8;
        }
    };

    struct SetPaletteColor
    {
        uint16_t futureUse; // 10 bits
        uint8_t colorIdx; // 8 bits
        uint8_t red; // 8 bits
        uint8_t green; // 8 bits
        uint8_t blue; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 10, futureUse);
            bitOffset += 10;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, colorIdx);
            bitOffset += 8;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, red);
            bitOffset += 8;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, green); 
            bitOffset += 8;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, blue);
            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 10, futureUse);
        }
    };

    struct DrawZOrderPixels
    {
        uint8_t drawMode; // 2 bits
        uint16_t zStart; // 16 bits
        uint16_t zEnd; // 16 bits
        uint8_t color; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 2, drawMode);
            bitOffset += 2;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, zStart);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, zEnd);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 2, drawMode);
            bitOffset += 2;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, zStart);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, zEnd);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }
    };

    struct DrawXYPixel
    {
        uint16_t rayIdx; // 10 bits
        uint8_t ledIdx; // 8 bits
        uint8_t color; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 10, rayIdx);
            bitOffset += 10;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, ledIdx);
            bitOffset += 8;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 10, rayIdx);
            bitOffset += 10;

            SetBitCompressedValue(data, dataLen, bitOffset, 8, ledIdx);
            bitOffset += 8;

            SetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }
    };

    struct DrawRect
    {
        uint8_t drawMode; // 2 bits
        uint16_t xPos; // 16 bits
        uint16_t yPos; // 16 bits
        uint16_t width; // 16 bits
        uint16_t height; // 16 bits
        uint8_t color; // 8 bits

        void DecodeFromBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            GetBitCompressedValue(data, dataLen, bitOffset, 2, drawMode);
            bitOffset += 2;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, xPos);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, yPos);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, width);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 16, height);
            bitOffset += 16;

            GetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }

        void EncodeToBitStream(uint8_t *data, size_t dataLen, size_t &bitOffset)
        {
            SetBitCompressedValue(data, dataLen, bitOffset, 2, drawMode);
            bitOffset += 2;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, xPos);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, yPos);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, width);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 16, height);
            bitOffset += 16;

            SetBitCompressedValue(data, dataLen, bitOffset, 8, color);
            bitOffset += 8;
        }
    };
};