#ifndef WEB_H
#define WEB_H
#include "headers/settings.h"
#ifdef TARGET_ESP8266
    #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "static/index.html.gz.h"

void setSettingsChangedHandler(void (*f)(Settings settings));

void webSetupHandlers();

void webSetup();

#endif
