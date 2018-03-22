#include <Wire.h>
String stringOne;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);         
}
 
void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
if (error == 0)
  {
  String stringOne =  String(address, HEX);
  Serial.print("0x");     Serial.print(stringOne); Serial.print(" - ");
    if(stringOne=="0A") Serial.println("'Motor Driver'");
    if(stringOne=="0F") Serial.println("'Motor Driver'");
    if(stringOne=="1D") Serial.println("'ADXL345 Input 3-Axis Digital Accelerometer'");
    if(stringOne=="1E") Serial.println("'HMC5883 3-Axis Digital Compass'");
    if(stringOne=="5A") Serial.println("'Touch Sensor'");
    if(stringOne=="5B") Serial.println("'Touch Sensor'");
    if(stringOne=="5C") Serial.println("'BH1750FVI digital Light Sensor' OR 'Touch Sensor"  );
    if(stringOne=="5D") Serial.println("'Touch Sensor'");
    if(stringOne=="20") Serial.println("'PCF8574 8-Bit I/O Expander' OR 'LCM1602 LCD Adapter' ");   
    if(stringOne=="21") Serial.println("'PCF8574 8-Bit I/O Expander'");
    if(stringOne=="22") Serial.println("'PCF8574 8-Bit I/O Expander'");
    if(stringOne=="23") Serial.println("'PCF8574 8-Bit I/O Expander' OR 'BH1750FVI digital Light Sensor'");
    if(stringOne=="24") Serial.println("'PCF8574 8-Bit I/O Expander'");
    if(stringOne=="25") Serial.println("'PCF8574 8-Bit I/O Expander'");
    if(stringOne=="26") Serial.println("'PCF8574 8-Bit I/O Expander'");
    if(stringOne=="27") Serial.println("'PCF8574 8-Bit I/O Expander' OR 'LCM1602 LCD Adapter '");   
    if(stringOne=="39") Serial.println("'TSL2561 Ambient Light Sensor'");    
    if(stringOne=="40") Serial.println("'BMP180 barometric pressure sensor'"    ); 
    if(stringOne=="48") Serial.println("'ADS1115 Module 16-Bit'");
    if(stringOne=="49") Serial.println("'ADS1115 Module 16-Bit' OR 'SPI-to-UART'");
    if(stringOne=="4A") Serial.println("'ADS1115 Module 16-Bit'");
    if(stringOne=="4B") Serial.println("'ADS1115 Module 16-Bit'");
    if(stringOne=="50") Serial.println("'AT24C32 EEPROM'"); 
    if(stringOne=="53") Serial.println("'ADXL345 Input 3-Axis Digital Accelerometer'");
    if(stringOne=="68") Serial.println("'DS3231 real-time clock' OR 'MPU-9250 Nine axis sensor module'");
    if(stringOne=="7A") Serial.println("'LCD OLED 128x64'");
    if(stringOne=="76") Serial.println("'BMP280 barometric pressure sensor'");
    if(stringOne=="77") Serial.println("'BMP180 barometric pressure sensor' OR 'BMP280 barometric pressure sensor'");
    if(stringOne=="78") Serial.println("'LCD OLED 128x64'" );
   nDevices++;
  }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);          
}