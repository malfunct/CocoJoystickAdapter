#include <SPI.h>
#include <XBOXUSB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
XBOXUSB Xbox(&Usb);

int SS0 = 4;
int SS1 = 5;
int button1 = 3;
int button2 = 2;

void setup() {
  
  Serial.begin(9600);
  
  if (Usb.Init() == -1) {
    Serial.print("USB fail");
    while (1); //halt
  }
  
  pinMode (SS0, OUTPUT);
  pinMode (SS1, OUTPUT);
  digitalWrite(SS0, HIGH);
  digitalWrite(SS1, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  pinMode(button1,OUTPUT);
  pinMode(button2,OUTPUT);

}

void loop() {
  Usb.Task();
  if (Xbox.Xbox360Connected) {
    int leftHatX = Xbox.getAnalogHat(LeftHatX);
    int leftHatY = Xbox.getAnalogHat(LeftHatY);

    // create a dead zone in the middle
    if(leftHatX < 0) {leftHatX += 4000; if(leftHatX > 0) leftHatX = 0;}
    if(leftHatX > 0) {leftHatX -= 4000; if(leftHatX < 0) leftHatX = 0;}
    if(leftHatY < 0) {leftHatY += 4000; if(leftHatY > 0) leftHatY = 0;}
    if(leftHatY > 0) {leftHatY -= 4000; if(leftHatY < 0) leftHatY = 0;}

    // allow corners to be max/max, introduces dead zones top, bottom, left, right
    if(leftHatX > 18000) leftHatX = 18000;
    if(leftHatX < -18000) leftHatX = -18000;
    if(leftHatY > 18000) leftHatY = 18000;
    if(leftHatY < -18000) leftHatY = -18000;
    
//    Serial.print("X hat: ");
//    Serial.print(leftHatX);
//    Serial.print(" Y hat: ");
//    Serial.println(leftHatY);

    int aButton = Xbox.getButtonPress(A);
    int bButton = Xbox.getButtonPress(B);
    int xButton = Xbox.getButtonPress(X);
    int yButton = Xbox.getButtonPress(Y);

    int tandyX = 255 - map(leftHatX,-18000,18000,0,255);
    int tandyY = map(leftHatY,-18000,18000,0,255);
    
//    Serial.print("TandyX: ");
//    Serial.print(tandyX);
//    Serial.print(" TandyY: ");
//    Serial.println(tandyY);
    
    int press1=LOW;
    int press2=LOW;
    if(aButton or xButton)
    {
      press1=HIGH;
    }
    if(bButton or yButton)
    {
      press2=HIGH;
    }

    MCP41010Write(tandyX,SS1);
    MCP41010Write(tandyY,SS0);
    digitalWrite(button1,press1);
    digitalWrite(button2,press2);
  }
  else
  {
    Serial.println("Not connected.");
  }
}

void MCP41010Write(byte value, int p)
{  // Note that the integer vale passed to this subroutine
  // is cast to a byte

  digitalWrite(p, LOW);
  SPI.transfer(B00010001); // This tells the chip to set the pot
  SPI.transfer(value);     // This tells it the pot position
  digitalWrite(p, HIGH);
}
