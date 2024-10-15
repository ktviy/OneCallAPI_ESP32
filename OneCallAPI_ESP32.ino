
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

//// Wifi ////
#define SSID_WIFI "AmericanStudy"
#define PASS_WIFI "66668888"

//// OpenWeather ////
#define LATITUDE "21.0787038"    // Ex: "10.77"
#define LONGITUDE "105.8656097"  // Ex: "106.65"
#define API_KEY "e6fa53bea61f102d5f3b16a80d468d22"
//
// Check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
#define TIME_UPDATE 30000UL  // Unit (ms)

//// API "Current weather data" ////
#define NAME_URL "https://api.openweathermap.org/data/3.0/onecall"
#define PARAM_LAT "?lat="
#define PARAM_LON "&lon="
#define PARAM_EXCLUDE "&exclude=minutely,hourly,daily"
#define PARAM_API "&appid="

//String URL = String(NAME_URL) + String(PARAM_LAT) + String(LATITUDE) + String(PARAM_LON) + String(LONGITUDE) + String(PARAM_EXCLUDE)+ String(PARAM_API) + String(API_KEY) ;
String URL = "https://api.openweathermap.org/data/3.0/onecall?lat=21.018605&lon=105.7958367&appid=e6fa53bea61f102d5f3b16a80d468d22";


//// JSON ////
String jsonBuffer;
JSONVar myObject;



void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID_WIFI, PASS_WIFI);
  Serial.println(F("Connecting"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));
  //
  Serial.print(F("Connected to WiFi network with IP Address: "));
  Serial.println(WiFi.localIP());
  //
  Serial.print(F("Pls, wait in about "));
  Serial.print(TIME_UPDATE / 1000);
  Serial.println(" seconds");

  
}


void loop() {
  /* ---------------- Send an HTTP GET request --------------- */
  if ((millis() - lastTime) >= TIME_UPDATE) {
    /* ------------- Check WiFi connection status ------------ */
    if (WiFi.status() == WL_CONNECTED) {
      jsonBuffer = httpGETRequest(URL.c_str());
       Serial.println(jsonBuffer);
      myObject = JSON.parse(jsonBuffer);

      // Check received JSON packet has data?
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println(F("Parsing input failed!"));
        return;
      }

      /* ----------------------------------------------------- */

      Serial.print("Weather: ");
      Serial.print(myObject["current"]["weather"][0]["main"]);
      Serial.print(" - ");
      Serial.println(myObject["current"]["weather"][0]["description"]);

      /* ----------------------------------------------------- */

      Serial.print("Temperature: ");
      Serial.print(myObject["current"]["temp"]);
      Serial.println("°C");
      //
      Serial.print("Humidity: ");
      Serial.print(myObject["current"]["humidity"]);
      Serial.println("%");
      //
      Serial.print("Feels Like: ");
      Serial.print(myObject["current"]["feels_like"]);
      Serial.println("°C");

      /* ----------------------------------------------------- */

      Serial.print("Wind Speed: ");
      Serial.print(myObject["current"]["wind_speed"]);
      Serial.println("m/s");
      //
      Serial.print("Wind Direction: ");
      Serial.print(myObject["current"]["wind_deg"]);
      Serial.println("°");

      /* ----------------------------------------------------- */

      Serial.print("Cloudiness: ");
      Serial.print(myObject["current"]["clouds"]);
      Serial.println("%");

      /* ----------------------------------------------------- */
      /* ----------------------------------------------------- */

      Serial.print("UV Index: ");
      Serial.println(myObject["current"]["uvi"]);
   
      /* ----------------------------------------------------- */

      Serial.print("Pressure: ");
      Serial.println(myObject["current"]["pressure"]);
      
      //
      Serial.print("TimeUpdate: ");
      Serial.print(myObject["current"]["dt"]);
      Serial.println("s");
      

      Serial.println(F(""));
    } else {
      Serial.println(F("WiFi Disconnected"));
    }
    /* ------------------------------------------------------- */
    lastTime = millis();
  }

  //displayOled();  // Show current weather information
}

/* ----------------------------------------------------------- */
/*                           FUNCTION                          */
/* ----------------------------------------------------------- */

String httpGETRequest(const char *serverName) {
  WiFiClientSecure client;  // Sử dụng WiFiClientSecure cho HTTPS
  HTTPClient http;

  client.setInsecure();  // Tạm thời bỏ qua việc xác thực chứng chỉ SSL

  // Bắt đầu yêu cầu HTTPS
  http.begin(client, serverName);

  // Gửi yêu cầu GET
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print(F("HTTP Response code: "));
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print(F("Error code: "));
    Serial.println(httpResponseCode);
  }

  // Giải phóng tài nguyên
  http.end();

  return payload;
}

/* ----------------------------------------------------------- */
