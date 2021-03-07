# CocoJoystickAdapter
Adapter to connect USB devices, such as gamepad or mouse,  to Tandy Color Computer joystick port.

Gerbers for shield PCB available in Shield Eagle Project directory. I have used https://jlcpcb.com/ to produce the boards in the past but any board house should be fine.

Components for the shield are listed in PartsList.txt

Arduino code for wireless xbox 360 controllers using wireless xbox 360 USB dongle is in the XBOX_360_wireless_arduino_code directory

Arduino code for wired xbox 360 controllers is in the XBOX_360_arduino_code directory

Make sure you have the libraries listed in Dependencies.txt installed for your Arduino IDE as well as configuring it for Arduino Uno R3 then open the correct ino file (wired or wireless) to match the controller you plan to use and compile and push to your Arduino Uno R3.

I assemble the shields with arduino on the bottom, USB sheild next and Coco Joystick shield on top. I power the arduino through the USB-B connector on the R3 board, plug my xbox controller into the USB sheild and use a straight through 6 pin din to 6 pin din cable to connect the adapter to the coco. A Commodore IEC cable works well for this purpose and are easy to find pre-built.

Have fun with your Coco!
