
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "API.h"
#include "display.h"

//https://randomnerdtutorials.com/esp32-http-get-post-arduino/

void readWeatherAPI(void){
  //https://open-meteo.com/en/docs
  char* API = "https://api.weatherapi.com/v1/current.json?key=86a605c5ebbd4c40a09135726210410&q=Barcelona";
  char* timeAPI = "https://timeapi.io/api/time/current/zone?timeZone=";

  HTTPClient http;
  http.begin(API);

  int httpCode = http.GET();

  if (httpCode <=0) {
    //printText("Connection proglem...", 10, 10);
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    return;
  }

  String payload = http.getString();
  Serial.println("Received payload:");
  Serial.println(payload);

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

