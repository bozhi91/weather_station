
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>

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

    Serial.printf("Content: %s ",json_buffer);

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

void readWeatherAPI(Weather_Data* forecast, int size){

  char* WEATHER_API = "https://www.meteosource.com/api/v1/free/point?place_id=";
  char* location    = "barcelona";
  char* query       = "&sections=daily,current&language=en&units=metric&key=";
  char* API_KEY     = "pi2hnter7xd66ujysijpn5g9jfl4s37ayp6v4yx2";

  char API_URL[256];
  char *json_data;
  int httpCode = 0;

  json_data = (char*)malloc(8192);

  memset(json_data, 0, sizeof json_data);
  sprintf(API_URL, "%s%s%s%s",WEATHER_API, location, query, API_KEY);
  Serial.println(" -> Requesting weather API data... ");

  httpCode = getHttpData(API_URL, json_data);

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf(" ERROR! HTTP Code: %d \n", httpCode);
    return;
  }

  //Parse the json data

  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(10) + 600;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, json_data);

  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  float max_temp = 0;
  float min_temp = 0;

  for(int i=0;i<size; i++){

    max_temp = doc["daily"]["data"][i]["all_day"]["temperature_max"];
    min_temp = doc["daily"]["data"][i]["all_day"]["temperature_min"];

    forecast[i].max_temp = (int)ceil(max_temp);
    forecast[i].min_temp = (int)ceil(min_temp);
    forecast[i].icon_id  = doc["daily"]["data"][i]["icon"];

    char date[12];
    strcpy(date, doc["daily"]["data"][i]["day"]);
    strncpy(forecast[i].summary, doc["daily"]["data"][i]["summary"], sizeof forecast[i].summary);
    memcpy(forecast[i].date,     &date[5], 5);

    Serial.printf("Temperature: %d|%d°C | icon: %d | date: %s | %s \n\n",
                  forecast[i].max_temp, forecast[i].min_temp,
                  forecast[i].icon_id,  forecast[i].date,
                  forecast[i].summary);
  }

  free(json_data);

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

int day_of_week(int d, int m, int y) {
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int K = y % 100;
    int J = y / 100;
    int h = (d + (13*(m + 1))/5 + K + K/4 + J/4 + 5*J) % 7;
    return ((h + 6) % 7); // 0=domingo, 1=lunes, ..., 6=sábado
}
