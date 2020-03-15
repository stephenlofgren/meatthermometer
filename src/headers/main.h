#ifndef MAIN_H
#define MAIN_H

#ifdef TARGET_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef TARGET_ESP32
#include <WiFi.h>
#include <esp_sleep.h>
#endif

#define BLYNK_PRINT Serial

void loopRun();
void loopConfig();

#ifdef TARGET_ESP8266
    int SENSOR_POWER = D1;
    int DO_CONFIG = D0;
#endif
#ifdef TARGET_ESP32
    int SENSOR_POWER = A3;
    int DO_CONFIG = A0;
#endif
#endif