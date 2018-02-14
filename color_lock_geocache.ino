/******************************************************************************
color_lock_geocache.ino
A lockbox consisting of an ISL29125 RGB Color Sensor from SparkFun and a simple servo.
By Megan Hayes-Golding January 2018
Based heavily on ISL29125_basics.ino by Jordan McConnell @ SparkFun Electronics & 
Sweep by Scott Fitzgerald 
https://github.com/sparkfun/ISL29125_Breakout 
https://www.arduino.cc/en/Tutorial/Sweep

Pinout for the RGB sensor
Sensor  Arduino
GND     GND
3.3V    3.3V
SDA     A4
SCL     A5
INT     don't connect

Pinout for the servo
Servo   Arduino
Red     5V
Brown   GND
Orange  D7

This code is beerware.
Distributed as-is; no warranty is given. 
******************************************************************************/
#include <Servo.h> // library for controlling a servo
#include <Wire.h> // I have no idea, tbh
#include <SFE_ISL29125.h> // library for the RGB color sensor

Servo myservo; // create a servo object for the lock/unlock mechanism
int pos = 0; // variable to store the servo position

SFE_ISL29125 RGB_sensor; // declare sensor object

float percentRed = 0; 
float percentBlue = 0;
float percentGreen = 0;

void setup()
{
  myservo.attach(7); // attaches the servo to digital pin 7
  
  Serial.begin(115200); // Initialize serial communication for debugging

  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }

  myservo.write(15); // set the servo to the locked position, if it wasn't already
}

// Read sensor values for each color and print them to serial monitor
void loop()
{
  // Create the counter to track where we are in the sequence
  int counter = 0;
  String color = "none";
  
  while(counter<4){
    // Read sensor values (16 bit integers)
    unsigned int red = RGB_sensor.readRed();
    unsigned int green = RGB_sensor.readGreen();
    unsigned int blue = RGB_sensor.readBlue();
    percentRed = (float)red/(red+green+blue)*100;
    percentGreen = (float)green/(red+green+blue)*100;
    percentBlue = (float)blue/(red+green+blue)*100;
    
    // Name the colors
    if(percentRed > 36 && percentRed < 46 && percentGreen > 40 && percentGreen < 50 && percentBlue > 10 && percentBlue < 20)
    {
      color = "yellow";
    }
    if(percentRed > 15 && percentRed < 25 && percentGreen > 38 && percentGreen < 48 && percentBlue > 31 && percentBlue < 41)
    {
      color = "green";
    }
    if(percentRed > 15 && percentRed < 25 && percentGreen > 38 && percentGreen < 49 && percentBlue > 31 && percentBlue < 41)
    {
      color = "turquoise";
    }
    if(percentRed > 60 && percentRed < 70 && percentGreen > 20 && percentGreen < 30 && percentBlue > 6 && percentBlue < 16)
    {
      color = "orange";
    }

    // Print out readings, change HEX to DEC if you prefer decimal output
    Serial.println();
    Serial.print("Red: "); Serial.print(red,DEC); Serial.print(" ("); Serial.print(percentRed); Serial.println("%)");
    Serial.print("Green: "); Serial.print(green,DEC); Serial.print(" ("); Serial.print(percentGreen); Serial.println("%)");
    Serial.print("Blue: "); Serial.print(blue,DEC); Serial.print(" ("); Serial.print(percentBlue); Serial.println("%)");
    Serial.print("Color: "); Serial.println(color);
    Serial.print("Counter: "); Serial.println(counter);

    if(counter == 0 && color == "yellow") // looking for yellow
    {
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); // flash the onboard LED once
      counter++;
    }
    if(counter == 1 && color == "green") // looking for green
    {
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500); digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); // flash twice
      counter++;
    } 
    if(counter == 2 && color == "turquoise") // looking for turquoise
    {
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500); 
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500); 
      digitalWrite(13, HIGH);delay(500); digitalWrite(13, LOW); // flash three times
      counter++;
    }
    if(counter == 3 && color == "orange") // looking for orange
    {
      counter++;
      myservo.write(180); // move the servo to position 180 unlocked
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500); 
      digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500); 
      digitalWrite(13, HIGH);delay(500); digitalWrite(13, LOW); // flash three times
      myservo.write(15); // move the servo to position 15 locked
      delay(5000);
      counter++;
    }
    delay(1000);
  }


  //This is the old way -- unlock on a single color
//  if (percentRed > 55 && red > 5)
//  {
//    Serial.print("Thank you for showing me something "); Serial.print(percentRed); Serial.println("% red.");
//   }
//  if (percentGreen > 55 && green > 5)
//  {
//    Serial.print("Thank you for showing me something "); Serial.print(percentGreen); Serial.println("% green.");
//  }
//  if (percentBlue > 55 && blue > 5)
//  {
//    Serial.print("Thank you for showing me something "); Serial.print(percentBlue); Serial.println("% blue.");
//    myservo.write(180); // move the servo to position 180 unlocked
//    digitalWrite(13, HIGH); // indicate the servo should be turning
//    delay(1000);
//    myservo.write(15); // move the servo to position 10 locked
//    digitalWrite(13, LOW); // indicate the servo should be turning
//    delay(5000); // you just opened the lock, no need to go opening it right away again
//
//  }
//  delay(1000);
//  
//  

  


}
