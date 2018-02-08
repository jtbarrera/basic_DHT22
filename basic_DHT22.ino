/*
Code for NodeMCU:
Tempature
humidity

output:
print to serial line

Original Code and reference material
https://github.com/bruhautomation/ESP-MQTT-JSON-Multisensor
https://www.losant.com/blog/getting-started-with-the-esp8266-and-dht22-sensor
*/

#include <DHT.h>

/**************************** PIN DEFINITIONS *******************************************/
#define DHTPIN    4
#define DHTTYPE   DHT22

/**************************** SENSOR DEFINITIONS ****************************************/
int calibrationTime = 0;
DHT dht(DHTPIN, DHTTYPE);

/********************************** START SETUP*****************************************/
void setup() {

  Serial.begin(115200);
  Serial.setTimeout(2000);
  
  // Wait for serial to initialize.
  while(!Serial) { }
  
  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

  Serial.begin(115200);
  delay(10);
}

/********************************** MAIN LOOP*******************************************/
int timeSinceLastRead = 0;
void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}
