#pragma once
#include "mbed.h"
#include "uop_msb.h"
#include "NTPClient.h"

extern NetworkInterface *_defaultSystemNetwork;
bool connect();
bool setTime();

