#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>

const char* ssid = "my_ssid";
const char* password = "my_pass";

WiFiServer server(80);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  bool status;
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  delayTime = 1000;
  Serial.println();
  printValues();

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  client.println("T;P;H");
  client.print(bme.readTemperature());
  client.print(";");
  client.print(bme.readPressure() / 133.3 );
  client.print(";");
  client.print(bme.readHumidity());
  client.println("");
  delay(10);
  client.stop();
  Serial.println("Client disonnected");
}

void printValues() {
  Serial.println("T;P;H");
  Serial.print(bme.readTemperature());
  Serial.print(";");
  Serial.print(bme.readPressure() / 133.3 );
  Serial.print(";");
  Serial.print(bme.readHumidity());
  Serial.println("");
}