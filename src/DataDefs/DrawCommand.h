#pragma once

#include <Arduino.h>

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
    };

    struct SetTimingOffset
    {
        uint32_t offsetNanoseconds; // 24 bits
    };

    struct SetTimingScale
    {
        uint32_t timeScaleFactor; // 24 bits
    };

    struct ClearMemory
    {
        uint16_t padding; // 10 bits
    };

    struct SetZLevel
    {
        uint8_t quadrant; // 2 bits
        uint8_t zLevel; // 8 bits
    };

    struct SetPaletteColor
    {
        uint16_t futureUse; // 10 bits
        uint8_t colorIdx; // 8 bits
        uint8_t red; // 8 bits
        uint8_t green; // 8 bits
        uint8_t blue; // 8 bits
    };

    struct DrawZOrderPixels
    {
        uint8_t drawMode; // 2 bits
        uint16_t zStart; // 16 bits
        uint16_t zEnd; // 16 bits
        uint8_t color; // 8 bits
    };

    struct DrawXYPixel
    {
        uint8_t drawMode; // 2 bits
        uint8_t xPos; // 8 bits
        uint8_t yPos; // 8 bits
        uint8_t color; // 8 bits
    };

    struct DrawRect
    {
        uint8_t drawMode; // 2 bits
        uint8_t xPos; // 8 bits
        uint8_t yPos; // 8 bits
        uint8_t width; // 8 bits
        uint8_t height; // 8 bits
        uint8_t color; // 8 bits
    };
};