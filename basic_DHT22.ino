#include <DHTesp.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

/************ Variables ******************/
#define wifi_ssid "" //type your WIFI information inside the quotes
#define wifi_password ""
#define SENSORNAME "sensor"

DHTesp dht;
WiFiClient espClient;

const int BUFFER_SIZE = 128;
int calibrationTime = 0;

float diffTEMP = 0.5;
float tempValue;
float diffHUM = 1;
float humValue;

/********************************** START SETUP*****************************************/
void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();  Serial.println("Starting Node named " + String(SENSORNAME));

  setup_wifi(); 
  dht.setup(4); // data pin 4
}

/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/********************************** BUILD JSON AND SEND********************************/
void sendState() {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["comfort"] = dht.getStatusString();
  root["humidity"] = humValue;
  root["temperature"] = dht.toFahrenheit(tempValue);
  root["heatIndex"] =   dht.computeHeatIndex(dht.toFahrenheit(tempValue), humValue, true);

  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));
  Serial.println(buffer);
}

/********************************** CHECK SENSOR DIFFERNCE ENOUGH TO SEND*************/
bool checkBoundSensor(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

/********************************** START MAIN LOOP***************************************/
void loop()
{
      
  float newHumValue = dht.getHumidity();
  float newTempValue = dht.getTemperature();

 if (checkBoundSensor(newTempValue, tempValue, diffTEMP)) {
   tempValue = newTempValue;
   sendState();
 }
 if (checkBoundSensor(newHumValue, humValue, diffHUM)) {
   humValue = newHumValue;
   sendState();
 }
}
