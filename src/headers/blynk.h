#ifndef BLYNK_H
    #define BLYNK_H
    #include <Arduino.h>  // for type definitions
    void blynkSetup();
    void blynkLoop(int sensorReading);
    void println(String message);
#endif