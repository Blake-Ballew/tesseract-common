#pragma once

namespace TesseractCommon
{
    // Opcodes for GPU commands. 6 bits
    enum 
    {
        CFG_ZLVL = 0x01, // SetStripZLevel
        CFG_OFS = 0x02,  // SetTimingOffset
        CFG_TSCL = 0x03, // SetTimingScale

        CTRL_CLR = 0x04, // ClearMemory
        CTRL_ZLQ = 0x05, // SetZLevelQuad
        CTRL_COLOR = 0x06, // SetPaletteColor

        DRW_ZORD = 0x07, // DrawZOrderPixels
        DRW_XY_PXL = 0x08, // DrawXYPixel
        DRW_XY_RECT = 0x09, // DrawRect

    } DrawCommandOpcode;

    
};