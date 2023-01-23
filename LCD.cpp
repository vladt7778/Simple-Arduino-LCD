#include "LCD.h"

LCD::LCD(uint8_t cols, uint8_t rows, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t brightnessPin, uint8_t contrastPin) : ROWS(rows), COLS(cols), BRIGHTNESS_PIN(brightnessPin), CONTRAST_PIN(contrastPin)
{
    m_DisplayInstance = new LiquidCrystal(rs, enable, d4, d5, d6, d7);

    m_LastTextSize = new uint8_t[rows];
    m_LastText = new char *[rows];

    for (uint8_t i = 0; i < rows; ++i)
    {
        m_LastText[i] = new char[cols + 1];
    }

    byte fullRectangle[] = {
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};

    m_DisplayInstance->createChar(0, fullRectangle);

    pinMode(brightnessPin, OUTPUT);
    pinMode(contrastPin, OUTPUT);
}

LCD::~LCD()
{
    if (m_DisplayInstance)
    {
        delete m_DisplayInstance;
    }

    if (m_LastTextSize)
    {
        delete[] m_LastTextSize;
    }

    if (m_LastText)
    {
        for (uint8_t i = 0; i < ROWS; ++i)
        {
            delete[] m_LastText[i];
        }

        delete[] m_LastText;
    }
}

uint8_t LCD::GetBrightness() const
{
    return m_Brightness;
}

uint8_t LCD::GetContrast() const
{
    return m_Contrast;
}

bool LCD::Visible() const
{
    return m_DisplayToggle;
}

void LCD::ClearLCD()
{
    if (!m_DisplayCleared)
    {
        for (uint8_t i = 0; i < ROWS; ++i)
        {
            memset(m_LastText[i], ' ', COLS);
            m_LastText[i][COLS] = '\0';
            m_DisplayInstance->setCursor(0, i), m_DisplayInstance->print(m_LastText[i]);
        }
        memset(m_LastTextSize, __UINT8_MAX__, ROWS * sizeof(*m_LastTextSize));
        m_DisplayCleared = true;
    }
}

void LCD::ClearLCD(uint8_t line, uint8_t startPos, uint8_t endPos)
{
    if (startPos != __UINT8_MAX__)
    {
        if (endPos > COLS - 1)
        {
            endPos = __UINT8_MAX__;
        }

        uint8_t max = endPos != __UINT8_MAX__ ? endPos + 1 : COLS;
        for (uint8_t i = startPos; i < max; ++i)
        {
            m_LastText[line][i] = ' ';
        }

        m_DisplayInstance->setCursor(0, line);
        m_DisplayInstance->print(m_LastText[line]);
    }
}

void LCD::Init()
{
    m_DisplayInstance->begin(COLS, ROWS);
    ResetDisplayView();
    ClearLCD();

    SetVisible(true);
}

void LCD::MapToValAndDrawProgress(uint8_t line, long inputValue, long rangeMin, long rangeMax, uint8_t minCol, uint8_t maxCol)
{
    ClearLCD(line, minCol);
    if (inputValue == 0)
    {
        return;
    }

    if (inputValue < rangeMin)
    {
        inputValue = rangeMin;
    }

    uint8_t maxColumn = map(inputValue, rangeMin, rangeMax, minCol, maxCol);

    if (maxColumn == 0)
    {
        maxColumn = 1;
    }

    for (uint8_t i = minCol; i < maxColumn; ++i)
    {
        m_DisplayInstance->setCursor(i, line);
        m_DisplayInstance->write(byte(0));
    }
}

void LCD::MapToValAndDrawProgress(uint8_t line, long inputValue, long rangeMin, long rangeMax)
{
    MapToValAndDrawProgress(line, inputValue, rangeMin, rangeMax, 0, COLS);
}

void LCD::ResetBrightness()
{
    m_Brightness = DEFAULT_BRIGHTNESS;
    analogWrite(BRIGHTNESS_PIN, DEFAULT_BRIGHTNESS);
}

void LCD::ResetContrast()
{
    m_Contrast = DEFAULT_CONTRAST;
    analogWrite(CONTRAST_PIN, DEFAULT_CONTRAST);
}

void LCD::ResetDisplayView()
{
    ResetBrightness();
    ResetContrast();
}

void LCD::SetBrightness(uint8_t brightness)
{
    this->m_Brightness = brightness;
    analogWrite(BRIGHTNESS_PIN, brightness);
}

void LCD::SetContrast(uint8_t contrast)
{
    this->m_Contrast = contrast;
    analogWrite(CONTRAST_PIN, contrast);
}

void LCD::SetVisible(bool toggle)
{
    if (!toggle)
    {
        m_DisplayInstance->noDisplay();
        analogWrite(BRIGHTNESS_PIN, 0);
        m_DisplayToggle = false;
    }
    else
    {
        m_DisplayInstance->display();
        analogWrite(BRIGHTNESS_PIN, m_Brightness);
        m_DisplayToggle = true;
    }
}

void LCD::ToggleVisibile()
{
    SetVisible(m_DisplayToggle);
}

void LCD::UpdateLCDText(uint8_t line, uint8_t pos, const char *text, bool clearRight)
{
    if (line > ROWS - 1)
    {
        return;
    }

    if (m_LastTextSize[line] != __UINT8_MAX__)
    {
        char newText[COLS + 1];
        memset(newText, ' ', COLS * (sizeof *newText));
        newText[COLS] = '\0';

        ReplaceText(newText + pos, text);

        if (!strcmp(newText, m_LastText[line]))
        {
            return;
        }
        uint8_t currLen = pos + strlen(text);

        ReplaceText(m_LastText[line] + pos, text);

        if (clearRight)
        {
            if (currLen < m_LastTextSize[line])
            {
                ClearLCD(line, currLen, m_LastTextSize[line]);
            }
        }
        m_LastTextSize[line] = currLen;
    }
    else
    {
        ReplaceText(m_LastText[line] + pos, text);
        m_LastTextSize[line] = COLS - strlen(text) + pos;
    }

    if (m_DisplayCleared)
    {
        m_DisplayCleared = false;
    }

    m_DisplayInstance->setCursor(pos, line);
    m_DisplayInstance->print(text);
}