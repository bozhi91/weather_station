
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Wifi_Module.h"

//https://randomnerdtutorials.com/esp32-http-get-post-arduino/

static Config config;

void getDeviceConfig(Config* conf){
  *conf = config;
}

/**
  Load device configuration from the json file.
*/
int loadDeviceConfig(void){
  
    unsigned char json_buffer[512];
    unsigned long size = 0;

    memset(json_buffer, 0, sizeof(json_buffer));
    memset(&config, 0, sizeof config);

    int fd = fread("/config.json", json_buffer, &size, 0);

    if(fd!=0){
      Serial.println("File read error");
    }

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, json_buffer);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return -1;
    }

    //Copy strings safely
    strlcpy(config.ssid, doc["ssid"] | "", sizeof(config.ssid));
    strlcpy(config.pass, doc["pass"] | "", sizeof(config.pass));
    strlcpy(config.lang, doc["lang"] | "", sizeof(config.lang));

    config.timezone    = doc["timezone"];    // 1 -> UTC+1 i.e. Spanish time 
    config.time_format = doc["time_format"]; // 21/24 hour format -> hh:mm 
    config.date_format = doc["date_format"]; // 0  -> dd/mm/yyyy; 1
   
    String tmp =  doc["temp_units"];
    config.temp_units = tmp.c_str()[0];
   // config.temp_units = doc["temp_units"][0];  

    Serial.printf("--> timezone: %d \n", config.timezone );
    Serial.printf("--> time_format: %d \n", config.time_format );
    Serial.printf("--> date_format: %d \n", config.date_format );
    Serial.printf("--> temp_units: %c \n", config.temp_units );

  return 0;
}

/*
    Returns a struct with the time/date values recovered from an external server.
    Time format is in UTC.
    The date/time format returned by the server has the following form: yyyy-mm-ddThh:mm:ss
    The date/time is formatted acording the
*/
void readTimeAPI(DateTime* date){
  
  char aux[2];
  int h;
  char response[64];
  char* timeAPI = "https://r1-api.dotdigital.com/v2/server-time";
  //"https://timeapi.io/api/time/current/zone?timeZone=Europe%2FMadrid";

  getHttpData(timeAPI, response);
  Serial.printf(" Time API Response: [%s]", response);

  //Format the received time and date accodring the stored time/date format and timezone
  aux[0] = response[12];
  aux[1] = response[13];
  h = atoi(aux);
  h = (h+config.timezone==24) ? 0 : h+config.timezone;

  sprintf(date->time, "%02d:%c%c",h,response[15],response[16]);
  sprintf(date->date, "%c%c/%c%c/%c%c%c%c", 
          response[9], response[10], 
          response[6], response[7], 
          response[1],response[2],response[3],response[4] );

  Serial.printf("date: %s|%s \n", date->date, date->time);

}

void readWeatherAPI(Current_weather* current){

  //https://open-meteo.com/en/docs
  //https://www.weatherapi.com/docs/
  //condition codes: https://www.weatherapi.com/docs/conditions.json
  //https://api.weatherapi.com/v1/forecast.json?key=86a605c5ebbd4c40a09135726210410&q=Barcelona&lang=en&dt=2025-06-14
  //const char* API_KEY = "milkulGrUGbG3q0idvGoMape8x0c9ARp"; //ACUWEATHER API KEY

  char* API_URL = "https://api.weatherapi.com/v1/forecast.json?key=86a605c5ebbd4c40a09135726210410&q=Barcelona&lang=en&dt=2025-06-";
  char url[256];

   Serial.println(" -> Requesting weather API...");

  sprintf(url, "%s%s", API_URL,current->date);

  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode <= 0) {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    return;
  }

  String payload = http.getString();
  Serial.printf("payload: %s \n ", payload);

  // Allocate JSON buffer
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(10) + 600;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {

    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  http.end();

    float max_temp = doc["forecast"]["forecastday"][0]["day"]["maxtemp_c"];
    float min_temp = doc["forecast"]["forecastday"][0]["day"]["mintemp_c"];
    char date[12];

    strcpy(date, doc["forecast"]["forecastday"][0]["date"]);
    memset(current->date, 0, 5);

    current->max_temp = (int)max_temp;
    current->min_temp = (int)min_temp;
    current->cond_id  =  doc["forecast"]["forecastday"][0]["day"]["condition"]["code"];
    memcpy(current->date, &date[5], 5);
    current->date[6] = '\0';

    Serial.printf("Current date; %s \n",current->date);

   /*char tmp[50];

    float tempC   = doc["current"]["temp_c"];
    float windKph = doc["current"]["wind_kph"];
    int humid     = doc["current"]["humidity"];
    int press     = doc["current"]["pressure_mb"];

    char data[100];
    int pos_y = 10;

    strncpy(tmp,  doc["location"]["name"], sizeof(tmp));
    sprintf(data,"City: %s", tmp);
    printText(data, 10, pos_y);
    pos_y+=30;

    sprintf(data, "Temp: %.1f C", tempC);
    printText(data, 10, pos_y);
    pos_y+=30;

    sprintf(data, "Humid: %d ", humid);
    printText(data, 10, pos_y);
    pos_y+=30;

    sprintf(data, "Press: %d mbar", press);
    printText(data, 10, pos_y);
    pos_y+=30;

    sprintf(data, "Wind: %.1f kph", windKph);
    printText(data, 10, pos_y);
    pos_y+=30;

    strncpy(tmp,  doc["location"]["localtime"], sizeof(tmp));
    sprintf(data, "Localtime: %s", tmp);
    printText(data, 10, pos_y);
    pos_y+=30;

    strncpy(tmp,  doc["location"]["tz_id"], sizeof(tmp));
    sprintf(data,"Timezone: %s", tmp);
    printText(data, 10, pos_y);
    pos_y+=30;*/

      // Print them
     /* Serial.println("====== Weather Info ======");
      Serial.printf("City: %s\n", city);
      Serial.printf("Time: %s \n", time);

      Serial.printf("Temperature: %.1f°C\n", tempC);
      Serial.printf("humid: %d %%\n", humid);
      Serial.printf("press: %d mbar\n", press);
      Serial.printf("Condition: %s\n", condition);
      Serial.printf("Wind: %.1f kph\n", windKph);

      Serial.println("==========================");*/
}

