 
#include <WiFi.h>
#include <SPI.h>
#include <HTTPClient.h>
#include "Wifi_Module.h"
#include "display.h"
#include "LayoutComponents.h"
#include "API.h"

static unsigned char wifi_status = 0;

/**
  Initialize the WIFI module.
  After a successfull connection, we check the local IP address, and the internet connection.

  TODO: Get the external ip address and the current country/region/city
*/
void initWifi(void){

  const char* ssid     = "";
  const char* password = "";
  unsigned long timer  = millis();
  Config conf;

  getConfig(&conf);
  
  wifi_status = 0;

  Serial.printf(" \n- Initializing WIFI module. Connecting to: %s \n", conf.ssid);

  //Initialize wifi module and connect to the specified Access Point
  WiFi.begin(conf.ssid, conf.pass);

  //Wait until we're connected to the Wifi AP or a 5s timeout has passed
  while(WiFi.status() != WL_CONNECTED && ((millis() - timer) < 10000));

  if(WiFi.status() != WL_CONNECTED){    
    msgBox("CONNECTION PROBLEM ", TYPE_ERROR);
    Serial.printf("\t [ FAILED ] \n");
    return;
  }
  wifi_status = 1;

  //Verify if we have an Internet connection and get the local IP
  String ip = WiFi.localIP().toString();

  char code = remoteConnCheck();
  Serial.printf("Local IP: %s. Internet connection: %s \n", ip.c_str(), (code == 204) ? "YES":"NO");

  //We're connected to the internet
  if(code == 204){
    wifi_status = 2;
  }
}

/*
  Check if we have an internet connection.
*/
char remoteConnCheck(void){

  HTTPClient http;
  http.begin("http://clients3.google.com/generate_204"); // Fast & lightweight URL
  int code = http.GET();
  http.end();

  return code; // 204 means success, no content
}

int getHttpData(char* url, char* outData){

  HTTPClient http;

  http.begin(url);            //Connect to the specified URL
  int httpCode = http.GET();  //Get the server response
  
  if (httpCode <=0) {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    httpCode;
  }

  String payload = http.getString();
  Serial.println("Received payload:");
  Serial.println(payload);
  
  http.end(); //Close the HTTP connection

  return httpCode;
}
