#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//Librerias para OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "../lib/ota.h"
#include "../lib/interfazweb.h"
#include "../lib/reloj.h"

//Librerias para OTA

conexionWeb *webInterface;


DateTime *reloj;
StaticJsonDocument<512> horaActual;
StaticJsonDocument<512> post_sembradios;
StaticJsonDocument<1024> get_ejemplo;

const char *urlPost = "http://192.168.0.10/esp32/public/api/registro";
const char *geturl = "http://192.168.0.10/app/public/api/resetpassword";

void setup()
{
  Serial.begin(115200);
  setupOTA("TemplateSketch");

  webInterface = new conexionWeb(ssid, passwrd);
  reloj = new DateTime();

  pinMode(2, OUTPUT);
}
int sumatoria = 0;
int i;
void loop()
{
for (i=201; i<=250; i++){
#ifndef ESP32_RTOS
  ArduinoOTA.handle();
#endif
  sumatoria++;

  horaActual.clear();
  post_sembradios.clear();
  reloj->getTime();

  horaActual["hora"] = reloj->timeStringBuff;
  horaActual["Sumatoria"] = sumatoria;

  post_sembradios["idPlanta"] = i;
  post_sembradios["PH"] = random(10);
  post_sembradios["TempAgua"] = random(30);
  post_sembradios["Hmedad"] = random(30);
  post_sembradios["fecha"] = reloj->timeStringBuff; 
  
  webInterface->webPOST(post_sembradios, urlPost);
  //  webInterface->webGET(geturl);

  deserializeJson(get_ejemplo, webInterface->POSTresponse);
  serializeJsonPretty(get_ejemplo, Serial);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  // serializeJson(horaActual,Serial);
  }
}