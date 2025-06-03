
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "vodafoneBA1840";
const char* password = "6GMJ4TSTUGXH6LHQ";

//Your Domain name with URL path or IP address with path
//const char* serverName = "http://192.168.0.10:80/index.html";
const char* serverName = "http://worldtimeapi.org/api/timezone/Europe/Madrid";
const char* API = "https://api.weatherapi.com/v1/forecast.json?key=86a605c5ebbd4c40a09135726210410&q=Barcelona";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime   = 0;
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to: ");
  Serial.print(ssid);

  while(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network. Your IP Address is: ");
  Serial.println(WiFi.localIP());
  //Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

 //Process the GET response


 // Serial.print("Responde: ");
 // Serial.println(response);

 
//datetime
    
    /*  for (int i = 0; i < keys.length(); i++) {
       JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
      }*/
}

void getDateTime(){

  String response = httpGETRequest(serverName);
  
  JSONVar myObject = JSON.parse(response);
  JSONVar keys     = myObject.keys();
  JSONVar datetime = myObject[keys[2]];

   Serial.print("Time=");
   Serial.println(datetime);
}

void loop() {

 // getDateTime();
  delay(5000);
  
 /* //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
              
      sensorReadings = httpGETRequest(serverName);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        sensorReadingsArr[i] = double(value);
      }
      Serial.print("1 = ");
      Serial.println(sensorReadingsArr[0]);
      Serial.print("2 = ");
      Serial.println(sensorReadingsArr[1]);
      Serial.print("3 = ");
      Serial.println(sensorReadingsArr[2]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }*/
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
  //  Serial.print("HTTP Response code: ");
   // Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
