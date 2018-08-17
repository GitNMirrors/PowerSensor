"# PowerSensor" 

Our Power Sensor is an affordable piece of kit to check your infrared CO2 lasertube and keep track of it's power over time. 
At the moment we supply it as the Sensor only, you will have to hook it up to an Arduino flash our arduino sketch. 

## Contents:

- PowerSensor calibrated and labeled with it's id and calibration data. 
- Matching  JST SM 2,54mm plug with wires
- 4,7k Ohm Resistor 

## Specs

- usable for 20-150W infrared CO2 laser tubes.
- asolue accuracy: +-5W
- relative accuracy: <2W

## Therory of operation

To measure the power of a laser tube you fire the laser at a target of 
known thermal capacity for a know amount of time and measure the temperature increase that causes.
The power probe consits of a block of black anodized aluminium with a digital temperature sensor in it. 
The black anodizing absorbs the infrared laser beam and transfers the heat to the aluminum block. 
A 1-wire temperature sensor inside the block measures the rise in temperature while the laser is firing. 


## Wiring 

The JST SM receptical attached to the power seonsor only has 3 pins in a 4 wire housing for futre expansion. 

###Pinout 

Receptical on the probe   Colors on the plug:
1: OneWire		black
2: (for futre use)		green
3: Vcc (5V and 3,3V)		red
4: GND		blue



### Connecting to Arduino

Connect the probe as follows:
blue wire to any GND pin on your arduino
red wire to 5/3.3V
black to A0 

Now add 4,7k Ohm pullup resistor between the red and black wire. 

The green wire remains nconnected. 

## Software 

To write the software to your Arduino you will need the Arduino IDE:

https://www.arduino.cc/en/Main/Software

And the "DallasTemperature" library which can be installed via the library manager:
https://www.arduino.cc/en/Guide/Libraries

Download 










