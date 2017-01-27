// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  
  delay(250);
  
  Wire.beginTransmission(0x52);
  Wire.write(0x40);
  Wire.write(0x0);
  //Wire.write(0x0);
  //Wire.write(0x0);
  int r = Wire.endTransmission();
  Serial.println(r);
  
  Wire.beginTransmission(0x52);
  Wire.write(0xF0);
  Wire.write(0x55);
   r = Wire.endTransmission();
  Serial.println(r);

  Wire.beginTransmission(0x52);
  Wire.write(0xFB);
  Wire.write(0x00);
   r = Wire.endTransmission();
  Serial.println(r);  

  Mouse.begin();
}

int map(int val)
{
  if (val >4) val+=32;
  if (val <-4) val-=32;
  
  if (val>96) val+=64;
  if (val<-96) val-=64;
  
  return val /32;
  
}

void loop()
{

  byte lastbut=3;
  
  Wire.requestFrom(0x52, 6);    // request 6 bytes from slave device #2

  if (6 == Wire.available())    // slave may send less than requested
  { 
    int x = Wire.read();
    x=x - 128;
    
    int y = Wire.read();
    y=y - 131;
    
    byte gx = Wire.read();
    byte gy = Wire.read();
    byte gz = Wire.read();
    
    byte but = Wire.read();
    but = but & 3;
        
    Mouse.move(map(x), -map(y), 0);
    
    //c = (c ^ 0x17) + 0x17;
    #if 0
    Serial.print(x);  
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.println(but);
    #endif
  
    #if 0
    if (but != lastbut)
    {
      if ((lastbut &1) && !(but &1)) Mouse.press(MOUSE_LEFT);
      if (!(lastbut &1) && (but &1)) Mouse.release(MOUSE_LEFT);
      if ((lastbut &2) && !(but &2)) Mouse.press(MOUSE_RIGHT);
      if (!(lastbut &2) && (but &2)) Mouse.release(MOUSE_RIGHT);
      
      lastbut=but; 
    }
    #endif
    
    if (but & 1) Mouse.release(MOUSE_LEFT); else Mouse.press(MOUSE_LEFT);
    if (but & 2) Mouse.release(MOUSE_RIGHT); else Mouse.press(MOUSE_RIGHT);

  }
  Wire.beginTransmission(0x52);
  Wire.write(0x0);
  //Wire.write(0x0);
  //Wire.write(0x0);
  int r = Wire.endTransmission();
  
  //Serial.println();
  
  delay(10);
}
