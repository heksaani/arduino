/*!
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
 */ 

 // error rate in the humidity sensor +-3%
 // temperature 18-23 C and humidity 88<x<96
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int slowFanSpeed = 60;

// Define Arctic fan pins connected to the breadboard
// from  Arduino pins 3 and 4
const int fanPin1 = 3;
const int fanPin2 = 4; 

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows


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
  // Init SHT20 Sensor
  sht20.initSHT20();

  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
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
  analogWrite(fanPin1, slowFanSpeed);
  analogWrite(fanPin2, slowFanSpeed);

  // Increase fan speed if temperature or humidity is too high
  if (temperature > 50 || humidity > 96) {
    // Increase fan speed
    analogWrite(fanPin1, 255); // Set first fan to maximum speed
    analogWrite(fanPin2, 255); // Set second fan to maximum speed
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