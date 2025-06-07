 
#include <WiFi.h>
#include <SPI.h>
#include <HTTPClient.h>
#include "Wifi_Module.h"

/**
  Initialize the WIFI module.
  After a successfull connection, we check the local IP address, and the internet connection.

  TODO: Get the external ip address and the current country/region/city
*/
void initWifi(void){

  const char* ssid     = "vodafoneBA1840";
  const char* password = "6GMJ4TSTUGXH6LHQ";
  unsigned long timer = millis();

  Serial.printf(" \n- Initializing WIFI module. Connecting to: %s \n", ssid);

  //Initialize wifi module and connect to the specified Access Point
  WiFi.begin(ssid, password);

  //Wait until we're connected to the Wifi AP or a 5s timeout has passed
  while(WiFi.status() != WL_CONNECTED && ((millis() - timer) < 5000));

  if(WiFi.status() != WL_CONNECTED){
    Serial.printf("\t [ FAILED ] \n");
    return;
  }

  remoteConnCheck();
}

/*
  Check if we have an internet connection.
*/
char remoteConnCheck(void){

  //Verify if we have an Internet connection and get the local IP
  String ip = WiFi.localIP().toString();

  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin("http://clients3.google.com/generate_204"); // Fast & lightweight URL
  int code = http.GET();
  http.end();

  Serial.printf(" - Connected! Local IP: %s. Internet connection: %s \n", ip.c_str(), (code == 204) ? "YES":"NO");

  return (code == 204); // 204 means success, no content
}
