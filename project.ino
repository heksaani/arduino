/*!
 * ~~~Automated Green House ~~~


 // Temperature and Humidity:
 * @file  getHumidityAndTemperature.ino
 * @brief  DFRobot's SHT20 Humidity And Temperature Sensor Module as a base for automated green house 
 * @details  This example demonstrates how to read the user registers to display resolution and other settings.
 * @n  Uses the SHT20 library to display the current humidity and temperature.
 * @n  Open serial monitor at 9600 baud to see readings.
 * @n  Errors 998 if not sensor is detected. Error 999 if CRC is bad.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [Zhangjiawei](jiawei.zhang@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-12-03
 * @url  https://github.com/DFRobot/DFRobot_SHT20
 * 

 //
 */ 
 // error rate in the humidity sensor +-3%
 // temperature 18-23 C and humidity 88<x<96
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// this is the default speed of the fan to keep the air flowing
const int slowFanSpeed = 20;
// Arctic fans 2 both connected through bread board using PWM pin 9
const int fanPin1 = 9;


// To see the current temperature and humidity we use LCD screen 
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// for the light source we use 
// instructions for the from here: https://www.electronicshub.org/arduino-controlled-power-outlet/
// code and Arduino wiring from here: https://lastminuteengineers.com/one-channel-relay-module-arduino-tutorial/
const int RelayPin = 6;


/**
 * Hardware Connections:
 * -VCC = 3.3V
 * -GND = GND
 * -SDA = A4 (use inline 330 ohm resistor if your board is 5V)
 * -SCL = A5 (use inline 330 ohm resistor if your board is 5V)
 */
DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);


void setup()
{
  Serial.begin(9600);
  // Configures the specified pin to behave either as an input or an output
  // Set RelayPin as an output pin
	pinMode(RelayPin, OUTPUT);
  // Init SHT20 Sensor for temperature and humidity
  sht20.initSHT20();

  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(fanPin1, OUTPUT);
  delay(100);
  Serial.println("Sensor init finish!");
  /**
   * Check the current status information of SHT20
   * Status information: End of battery, Heater enabled, Disable OTP reload
   * Check result: yes, no
   */
  sht20.checkSHT20();
}


void loop() {
  // Read temperature and humidity
  float humidity = sht20.readHumidity();
  float temperature = sht20.readTemperature();
  // write the slow fan speed to pin
  analogWrite(fanPin1, slowFanSpeed);

  if(humidity < 85 ){
    digitalWrite(RelayPin, HIGH); // relay on, wiring is upside down = ) 
    delay(10000); 
  }
  if (humidity > 93) { 
    digitalWrite(RelayPin, LOW); // turns the relay of
  } 


  // Increase fan speed if temperature or humidity is too high
  if (temperature > 22 || humidity > 94) {
    // Increase fan speed
    analogWrite(fanPin1, 100); 
  }

  // Display temperature and humidity on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // Display temperature with one decimal place
  lcd.write(0xDF);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity, 1); // Display humidity with one decimal place
  lcd.print("%");

  // Delay for 5 seconds before looping back to displaying temperature and humidity
  delay(5000);

 
}
