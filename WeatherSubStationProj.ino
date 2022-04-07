   #include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ArduinoJson.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>


ESP8266WiFiMulti WiFiMulti;

/* Declare LCD object for SPI
  Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);*/
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 5, 15, 4); /*D5, D7, D1, D8, D2 */
int contrastValue = 60; /* Default Contrast Value */

#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 2000;

DHTesp dht;

const String SensorAPIUrl ="https://myweatherappiot.herokuapp.com/api/v1/sensor/"; 
//"http://192.168.1.25/api/v1/sensor";

// System Define Functions

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);

  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
  // use this instead:
  dht.setup(0, DHTesp::DHT11); // Connect DHT sensor to GPIO 17

  SetupDisplay();
  SetupWiFi();

}

void loop()
{

  // add function to get data from DHT 11 and display in screen

  getLocalWeatherStationDetails();

  getWeatherStationDetails();

  delay(1000);
  ClearDisplay();

}

// User Define Functions

void SetupDisplay()
{
  /* Initialize the Display*/
  display.begin();
  display.setCursor(0, 4);
  /* Change the contrast using the following API*/
  display.setContrast(contrastValue);

}

void SetupWiFi()
{
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();

    DisplayInScreen("[SETUP] WAIT WiFi Setup...", 5);

    delay(1000);
    ClearDisplay();

  }

  WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("Shreyash", "Otari@02");
    WiFiMulti.addAP("Rajeshwari", "rajeshwari12");
  
  
  
}


void ClearDisplay()
{

  /* Clear the buffer */
  display.clearDisplay();
  display.display();
  //delay(500);

}

void DisplayInScreen(String sData, int iUser)
{
  int iPos = 1;
  /* Now let us display some text */
  display.setTextSize(1);
  display.setTextColor(BLACK);


  if (iUser == 1) {
    iPos = 1;
     display.setTextSize(1);
      display.setCursor(5,5);
    
      display.println(sData);
      
      display.display();
      delay(500);
   
  }
  else if (iUser == 2)
  {
    iPos = 2;
  
     // display.setCursor(15, iPos);
    display.setCursor(5,5);
    display.println(sData);
    display.display();
    delay(500);

  
  }
  else if (iUser == 3)
  {
    iPos = 3;
  }
  else
  {
    iPos = 4;
  }

 
}

void getLocalWeatherStationDetails() {
  // Get DHT11 data and display in LCD screen
  delay(dht.getMinimumSamplingPeriod());
  String sWeatherData = "";
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

  sWeatherData =  "DHT11 Sensor\n\nhumidity: " + String(humidity, 0) + "%\ntemperature:" + String(temperature, 0) +"*C";
  DisplayInScreen(sWeatherData, 1);

  SendSensorDataToServer(temperature, humidity);

}

void getWeatherStationDetails() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

     std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setInsecure();
    
  //  WiFiClient client;

    HTTPClient https;

    Serial.print("[HTTP] begin...\n");
    if (https.begin(*client, SensorAPIUrl)) { // HTTPS


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          // Serial.println(payload);
          ParseJsonData(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(5000);
}

void SendSensorDataToServer(float SensorTemp, float SensorHumidity )
{
  Serial.println("Send Data to Sensor");
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
     // WiFiClient client;
    
     std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setInsecure();
    
      
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(*client, SensorAPIUrl);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST

      String httpRequestData = "{\"username\":\"raj\",\"email_id\":\"Maharastra\",\"devicename\":\"xyz1234\",\"temperature\":" + String(SensorTemp, 0) + ",\"humidity\":" + String(SensorHumidity, 0) + "}";
      // Send HTTP POST request
      Serial.println("Http send Data JSON");
      Serial.println(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

}




void ParseJsonData(String input)
{
  // String input;
Serial.println(input);
  DynamicJsonDocument doc(1536);

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
    delay(1000);
    ClearDisplay();
  for (JsonObject item : doc.as<JsonArray>()) {

    const String username = item["username"]; // "raj", "milind", "kunal", "surav", 
    const String devicename = item["devicename"]; // "asd258", "qwe369", "abc1234", "xyz123", "xyz1234"
    const String humidity = String(item["humidity"]); // "raj54@gamil.com", "mil@gmail.com", "k@gmail.com", ...
    const String temperature = item["temperature"]; // "24", "34", "25", "15", "15"
    //int humidity = item["humidity"]; // 70, 45, 65, 60, 10
    const String sensortime = item["sensortime"]; // "2022-08-21T13:00:00.000Z", "2022-08-26T13:00:00.000Z", ...

    Serial.println("User name =" + username + " : devicename =" + devicename);
    Serial.println("humidity =" + humidity + " : temperature = " + temperature );
    Serial.println(" sensortime = " + sensortime);

    //String sDataDisplay = username  + ":" + devicename + ":" + locationname + ":" + temperature ;
   // String sDataDisplay = username +" : "+ temperature +"*C : " + humidity ;
     String sDataDisplay = username +"\n\nhumidity: "+ humidity + "%\ntemperature: " + temperature +"*C";
    DisplayInScreen (sDataDisplay, 1);
    delay(1500);
    ClearDisplay();
  }

}
