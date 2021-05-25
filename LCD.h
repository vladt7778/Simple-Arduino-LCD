#pragma once
#include <LiquidCrystal.h>
#include "Utils.h"

class LCD
{
    constexpr static uint8_t DEFAULT_BRIGHTNESS = 255;
    constexpr static uint8_t DEFAULT_CONTRAST = 105;

    const uint8_t BRIGHTNESS_PIN;
    const uint8_t CONTRAST_PIN;
    const uint8_t COLS;
    const uint8_t ROWS;

    LiquidCrystal *m_DisplayInstance = nullptr;

    uint8_t m_Brightness;
    uint8_t m_Contrast;

    char **m_LastText = nullptr;

    uint8_t *m_LastTextSize = nullptr;

    bool m_DisplayCleared;
    bool m_DisplayToggle = true;

public:
    LCD(uint8_t rows, uint8_t cols, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t brightness, uint8_t contrast);
    ~LCD();

    uint8_t GetBrightness() const;
    uint8_t GetContrast() const;

    bool Visible() const;

    void ClearLCD();
    void ClearLCD(uint8_t line, uint8_t startPos, uint8_t endPos = __UINT8_MAX__);
    void Init();
    void ResetBrightness();
    void ResetContrast();
    void ResetDisplayView();
    void SetBrightness(uint8_t brightness);
    void SetContrast(uint8_t contrast);
    void SetVisible(bool toggle);
    void ToggleVisibile();
    void UpdateLCDText(uint8_t line, uint8_t pos, const char *text, bool clearRight = true);
};