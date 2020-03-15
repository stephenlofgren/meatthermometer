#include <Arduino.h>
#include "headers/main.h"
#include "headers/settings.h"
#include "headers/web.h"
#include "headers/temp.h"
#include <ver.h>
#include "headers/utilities.h"
#include "headers/message.h"
#include "headers/blynk.h"

void (*loopfcnPtr)();

void tempSetup(){
  setSensorPin(A0);
}

void wifiSetup(){
  Settings s = getSettings();
  if(settingsSet()){
    println("Connecting to Wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(s.ssid, s.password);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
    }
    String msg = "Wifi Connected " + WiFi.localIP().toString();
    println(msg);
  }
  else{
    println("Starting Accesspoint");
    WiFi.softAP(s.accessPointName);
    webSetup();
  }
}

void sleep(uint64 sleepSeconds = 0){
  uint64 sleep = sleepSeconds * 60000000;
  #ifdef TARGET_ESP8266
  if(sleep == 0){
    sleep = (uint64)_sensorSettings.sleepInterval * 60000000; 
  }
  //If we set the value too high use the max allowed value
  if(sleep > ESP.deepSleepMax() * .95)
    sleep = ESP.deepSleepMax() *.95;
  Serial.print("Going to sleep for ");
  Serial.print(uint64ToString(sleep));
  Serial.println(" microseconds");
  ESP.deepSleep(sleep);
  #endif
  #ifdef TARGET_ESP32
  if(sleep == 0)
    sleep = (uint64_t)1440 * 60000000;
  esp_sleep_enable_timer_wakeup(sleep);
    Serial.print("Going to sleep for ");
    Serial.print(uint64ToString(sleep));
    Serial.println(" seconds");
  esp_deep_sleep_start();
  #endif
}

void setupConfig(){
  println("Starting Accesspoint");
  Settings s = getSettings();
  WiFi.mode(WIFI_OFF);
  WiFi.softAP(s.accessPointName);
  webSetup();
}

void loopConfig(){
  println("Configuring");
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(180000);
  if(WiFi.softAPgetStationNum() == 0){
    sleep(1440);
  }
}

void loopRun(){
  println("starting loop");
  _sensorSettings = getSettings();
  digitalWrite(SENSOR_POWER, HIGH);
  float temp = readTemperature();
  digitalWrite(SENSOR_POWER, LOW);
  if(temp > -1) //probe attached
  {
    wifiSetup();
    blynkSetup();
    mqttSetup();
    Serial.println("setup complete");
    blynkLoop(temp);
    mqttLoop(temp);
    sleep();
    delay(1000);
  }
  else // probe detached put in config mode
  {
    sleep();
  }
}

bool shouldSleep(){
  pinMode(DO_CONFIG, INPUT); 
  int doConf = digitalRead(DO_CONFIG);
  Serial.println(doConf);
  return doConf == LOW && didWakeFromDeepSleep();
}

bool shouldConfig(){
  if(!settingsSet())
    return true;

  pinMode(DO_CONFIG, INPUT); 
  int doConf = digitalRead(DO_CONFIG);
  Serial.println(doConf);
  return doConf == LOW && !didWakeFromDeepSleep();
}

void setupSensor(){
    pinMode(SENSOR_POWER, OUTPUT); 
}

void setup() {
  Serial.setDebugOutput(true);
  Serial.begin(115200);
  delay(2000);
  
  if(shouldSleep()){
    Serial.println("Going to Sleep Right away");
    sleep(1440);//8266 will end up sleeping max
    return;
  }

  if(shouldConfig()){
    Serial.println("Starting Configuration");
    setupConfig();
    loopfcnPtr = loopConfig;
    return;
  }
  loopfcnPtr = loopRun;
}

void loop() {
  loopfcnPtr();
}


