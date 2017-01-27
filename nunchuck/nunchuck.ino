// Wii Nunchuck as USB HID mouse with Leonardo
// http https://sites.google.com/view/RandomProjects

// Created 25 January  2017

// MIT license

#include <Wire.h>

#define DEBUG 1
#define SIMULATE 1

int write2(byte a, byte b)
{
  Wire.beginTransmission(0x52);      // Nunchuck IIC address
  Wire.write(a);
  Wire.write(b);
  int r = Wire.endTransmission();
#ifdef DEBUG
  Serial.print("IIC write return code: ");
  Serial.println(r);  
#endif
  
  return r;
}

void setup()
{
  Wire.begin();        // Join IIC bus as master
  Serial.begin(9600);
  
  delay(250);        // let things settle
  
#ifdef DEBUG
  delay(5000);        // Give time to open the serial monitor
#endif

  write2(0x40,0x00);  // OEM initialization
  write2(0xF0,0x55);  // Alternate initialization, seems to work on clones
  write2(0xFB,0x00);

  Mouse.begin();    // start the mouse HID
}

// Give the mouse some acceleration the further it is pushed
int map(int val)
{
  if (val >4) val+=32;
  if (val <-4) val-=32;
  
  if (val>96) val+=64;
  if (val<-96) val-=64;
  
  return val /32;        // scale back down
}

void loop()
{
  // Get next report
  Wire.beginTransmission(0x52);
  Wire.write(0x0);
  Wire.endTransmission();
  
  Wire.requestFrom(0x52, 6);    // Request 6 bytes from nunchuck

  if (6 == Wire.available())    // ... may send less than requested
  { 
    // OEM device may need data 'decoding'
    // i.e. c = (c ^ 0x17) + 0x17;
    int x = Wire.read();
    int y = Wire.read();
    
    // Gyro data - could be used for airmouse
    byte gx = Wire.read();
    byte gy = Wire.read();
    byte gz = Wire.read();
    
    // Note buttons include the Gyro LSBs
    byte but = Wire.read();

#ifdef DEBUG
    Serial.print("Raw X,Y: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    
    Serial.print(" Buttons: ");
    Serial.println(but);
#endif

    // Adjust these for centerpoint
    x=x - 128;
    y=y - 131;   
    but = but & 3;        // Mask out the Gyro LSBs
    
    // When SIMULATE defined, don't actually move the mouse... 
    // Good idea to ensure data is good first
#ifndef SIMULATE
    Mouse.move(map(x), -map(y), 0);
    
    if (but & 1) Mouse.release(MOUSE_LEFT); else Mouse.press(MOUSE_LEFT);
    if (but & 2) Mouse.release(MOUSE_RIGHT); else Mouse.press(MOUSE_RIGHT);
#endif

  }
    
#ifdef DEBUG
  // Slow things down so we can see what's happening
  delay(500);
#endif

  delay(10);
}
