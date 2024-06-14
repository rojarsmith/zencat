#ifndef CASE3_SLICE_INCLUDED
#define CASE3_SLICE_INCLUDED

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "rs232.h"
#include "atagent.h"

struct at_agent esp32;

void case3_slice();

void case3_slice()
{
    esp32.hardware = ESP32;
    set_config_at_agent(&esp32);
}

#endif