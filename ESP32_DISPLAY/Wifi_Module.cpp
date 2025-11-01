 
#include <WiFi.h>
#include <SPI.h>
#include <HTTPClient.h>
#include "Wifi_Module.h"
#include "display.h"
#include "LayoutComponents.h"
#include "API.h"

static unsigned char conn_status = 0;

static char* str_conn_status[] = {
  "NOT CONNECTED",
  "CONNECTED TO WIFI AP",
  "CONNECTED TO INTERNET"
};

/**
  Initialize the WIFI module.
  After a successfull connection, we check the local IP address, and the internet connection.

  TODO: Get the external ip address and the current country/region/city
*/
int initWifi(void){

  unsigned long timer  = millis();
  Config conf;

  getDeviceConfig(&conf);
  conn_status = 0;

  Serial.printf(" \n- Initializing WIFI module. Connecting to AP: %s.... ", conf.ssid);

  //Initialize wifi module and connect to the specified Access Point
  WiFi.mode(WIFI_STA);               // Station mode
  WiFi.begin(conf.ssid, conf.pass);

  //Wait until we're connected to the Wifi AP or a 10s timeout has passed
  while (WiFi.status() != WL_CONNECTED && (millis() - timer < WIFI_CONN_TIMEOUT)) {
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\t[ CONNECTED ]\n");
    conn_status = 1;
  } 
  else {
    Serial.printf("\t[ TIMEOUT ]\n");
    return conn_status;
  }

  //Verify if we have an Internet connection and get the local IP
  //String ip = WiFi.localIP().toString();
  remoteConnCheck();

  //Serial.printf("Local IP: %s. Internet connection: %s \n", ip.c_str(), (conn_status == 2) ? "YES":"NO");
  getNetworkInfo();

  return conn_status;
}

void getNetworkInfo(void){

  Serial.printf("\n\n ===== DEVICE NETWORK INFO =====\n");

  Serial.printf(" - Connection status: %s \n", str_conn_status[conn_status]);
  Serial.printf(" - Connected to: %s\n", WiFi.SSID().c_str());
  Serial.printf(" - Local IP: %s \n", WiFi.localIP().toString());
  Serial.printf(" - Gateway: %s \n", WiFi.gatewayIP().toString());
  Serial.printf(" - MAC: %s \n", WiFi.macAddress().c_str());
  //Serial.printf(" - Signal (RSSI): %s", WiFi.RSSI());

   Serial.printf("\n\n");  
}

/*
  Check if we have an internet connection.
*/
char remoteConnCheck(void){

  int code = 0;
  char response[128];
  char* conn_url = "http://clients3.google.com/generate_204";

  code = getHttpData(conn_url, response);

  //We're connected to the internet
  if(code == 204){
    conn_status = 2;
  }
  return code; // 204 means success, no content
}

/**
  Sends a GET request to a given URL and returns the server response
 */
int getHttpData(char* url, char* outData){

  HTTPClient http;

  //IF the device is not connected to any device, there's no point of checking the URL
  if(conn_status == 0) {
    Serial.printf("Unable to connect to the URL: %s. No Internet connection! \n", url);
    return -1;
  }

  Serial.printf("\n Requesting URL: [%s] ", url);
  http.begin(url);            //Connect to the specified URL
  int httpCode = http.GET();  //Get the server response
  
  Serial.printf(" | Response: %d \n", httpCode);
  if (httpCode<=0) {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  strcpy(outData, http.getString().c_str());
  http.end(); //Close the HTTP connection

  return httpCode;
}

/**
  Returns the current connection status:

  0: Not connected
  1: Connected to the wifi router
  2: Connected to the Internet
 */
int getConnStatus(void){
  return conn_status;
}
