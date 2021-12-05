#pragma once
#include "Arduino.h"

typedef unsigned long ulong;

static inline void ReplaceText(char *destination, const char *source)
{
    if (destination == NULL)
    {
        return;
    }

    while (*destination++ = *source++);
}