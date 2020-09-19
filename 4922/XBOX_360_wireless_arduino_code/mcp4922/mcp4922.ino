#include <XBOXRECV.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <DAC_MCP49xx.h>


USB Usb;
XBOXRECV Xbox(&Usb);

const int CS = 4;
const int LDAC = 5;
int BUTTON1 = 2;
int BUTTON2 = 3;

DAC_MCP49xx dac(DAC_MCP49xx::MCP4922, CS, LDAC);

void setup() {  
  Serial.begin(115200);
  
  if (Usb.Init() == -1) {
    Serial.print("Failed.");
    while (1); //halt
  }
  
  dac.setSPIDivider(SPI_CLOCK_DIV16);
  pinMode(BUTTON1,OUTPUT);
  pinMode(BUTTON2,OUTPUT);
}

void loop() {
  Usb.Task();
  if (Xbox.Xbox360Connected) {
    int leftHatX = Xbox.getAnalogHat(LeftHatX);
    int leftHatY = Xbox.getAnalogHat(LeftHatY);

    int rightHatX = Xbox.getAnalogHat(RightHatX);
    int rightHatY = Xbox.getAnalogHat(RightHatY);

    // if you are using only the right stick replace
    // left stick values with right stick values to
    // enable the use of either stick
    if(abs(rightHatX) > 4000 && abs(leftHatX) <= 4000)
    {
      leftHatX == rightHatX;
    }

    if(abs(rightHatY) > 4000 && abs(leftHatY) <= 4000)
    {
      leftHatY == rightHatY;
    }

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

    int up = Xbox.getButtonPress(UP);
    int down = Xbox.getButtonPress(DOWN);
    int left = Xbox.getButtonPress(LEFT);
    int right = Xbox.getButtonPress(RIGHT);

    if(up)
    {
      leftHatY = 18000;
    }

    if(down)
    {
      leftHatY = -18000;
    }

    if(right)
    {
      leftHatX = 18000;
    }

    if(left)
    {
      leftHatX = -18000;
    }
    
    Serial.print("X hat: ");
    Serial.print(leftHatX);
    Serial.print(" Y hat: ");
    Serial.print(leftHatY);

    int aButton = Xbox.getButtonPress(A);
    int bButton = Xbox.getButtonPress(B);
    int xButton = Xbox.getButtonPress(X);
    int yButton = Xbox.getButtonPress(Y);

    int leftTrigger = Xbox.getButtonPress(L2);
    int rightTrigger = Xbox.getButtonPress(R2);

    int tandyX = map(leftHatX,-18000,18000,0,4095);
    int tandyY = 4095 - map(leftHatY,-18000,18000,0,4095);
    
    Serial.print(" TandyX: ");
    Serial.print(tandyX);
    Serial.print(" TandyY: ");
    Serial.print(tandyY);
    
    int press1=LOW;
    int press2=LOW;
    if(aButton or xButton or rightTrigger)
    {
      press1=HIGH;
    }
    if(bButton or yButton or leftTrigger)
    {
      press2=HIGH;
    }

    if(press1 == HIGH)
    {
      Serial.print(" Button 1");
    }

    if(press2 == HIGH)
    {
      Serial.print(" Button 2");
    }

    Serial.println("");
  
    dac.output2(tandyX,tandyY);
    dac.latch();
    digitalWrite(BUTTON1,press1);
    digitalWrite(BUTTON2,press2);
  }
  else
  {
    Serial.println("not connected");
  }
}
