#include "LCD.h"

LCD lcd(16, 2, 4, 2, 7, 8, 12, 13, 6, 9);

unsigned long last = 0;
unsigned long seconds = 0;

void setup()
{
	lcd.Init();
}

void loop()
{
	if (millis() - last > 999)
	{
		char buffer[10];
		ultoa(seconds++, buffer, 10);

		lcd.UpdateLCDText(0, 0, buffer, false);

		last = millis();
	}
}