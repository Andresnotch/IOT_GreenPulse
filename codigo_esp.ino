#include <HTTPClient.h>
#include <NewPing.h>
#include <DHT.h>
#include "WiFi.h"
#include <Wire.h>
#include <BH1750.h>

//pruebamoviles-e07f4
#define DHTPIN 32
#define DHTTYPE DHT11
#define SOIL_MOIST_PIN 34// %
#define LIGHT_PIN 35 // lux 
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

float lux=0.00, ADC_value=0.0048828125, LDR_value;
const char* ssid = "INFINITUMADN";
const char* password =  "4ndr35n07ch";


String serverName = "https://iot-0.herokuapp.com/plants";

void setup() {
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  delay(10 * 1000);
  HTTPClient http;

  // Quantity measures
  int temp  = dht.readTemperature(); 
  int env_hum = dht.readHumidity(); 
  // Soil moist has to be converted to percentage
  int soil_moist = analogRead(SOIL_MOIST_PIN);
  soil_moist = 100-map(soil_moist,0,4095,0,100);
  int light = analogRead(LIGHT_PIN);

  
  // Join measures
  String measure_url = serverName + "?humidity=" + env_hum + "&soil_moist=" + soil_moist + "&light=" + light + "&temperature=" + temp;

  // Send HTTP request
  http.begin(measure_url.c_str());
  int httpResponseCode = http.GET();

  if (httpResponseCode>0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx\n");

  Serial.print("Humidity: ");
  Serial.print(env_hum);
  
  Serial.print("Temperature: ");
  Serial.print(temp);

  //percentage
  Serial.print("Soil Moisture value: ");
  Serial.println(soil_moist);
  
  Serial.print("Light: ");
  Serial.println(light);
  delay(60 * 5 * 1000);
}
