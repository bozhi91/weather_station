
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "API.h"
#include "display.h"
#include "MemoryCard.h"
#include "hardware.h"
#include "Wifi_Module.h"

//https://randomnerdtutorials.com/esp32-http-get-post-arduino/

static Config config;
static DateTime_Cfg date_time_cfg = { 1, 0, 0 }; //TODO: load this config from the JSON file

void getConfig(Config* conf){
  *conf = config;
}

/**
  Load device configuration from the json file.
*/
int loadConfig(void){
  
    unsigned char json_buffer[512];
    unsigned long size = 0;

    memset(json_buffer, 0, sizeof(json_buffer));
    fread("/config.json", json_buffer, &size, 0);

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
    
    /*strlcpy(config.lang, doc["lang"] | "", sizeof(config.lang));
    strlcpy(config.timezone, doc["timezone"] | "", sizeof(config.timezone));

    // Copy integers and chars
    config.refresh_time = doc["refresh_time"] | 0;
    config.time_format = doc["time_format"] | 12;

    const char* temp = doc["temp_units"] | "C";
    config.temp_units = temp[0];  // Only first character (e.g. 'C')
    */

  return 0;
}

/*
    Returns a struct with the time/date values recovered from an external server.
    Time format is in UTC.
    The date/time format returned by the server has the following form: yyyy-mm-ddThh:mm:ss
    The date/time is formatted acording the
*/
void readTimeAPI(DateTime* date){

  char response[64];
  char* timeAPI = "https://r1-api.dotdigital.com/v2/server-time";
  //"https://timeapi.io/api/time/current/zone?timeZone=Europe%2FMadrid";

  getHttpData(timeAPI, response);
  Serial.printf("API Response: [%s]", response);

  //TODO: Need to format and validate the datra properly. Apply the corresponding timezone, etc.

  memcpy(date->date, &response[1], 10);
  memcpy(date->time, &response[12], 8);

  //date->time[1]+=date_time_cfg.timezone;

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

