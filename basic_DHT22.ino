#include <DHTesp.h>
#include <ArduinoJson.h>

/************ Variables ******************/
DHTesp dht;

const int BUFFER_SIZE = 128;

float diffTEMP = 0.5;
float tempValue;
float diffHUM = 1;
float humValue;

/********************************** START SETUP*****************************************/
void setup()
{
  Serial.begin(115200);
  Serial.println();
  dht.setup(4); // data pin 4
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
  delay(dht.getMinimumSamplingPeriod()); //for DHT22 it is 2000 ms
      
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
