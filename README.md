# Smoke and Mirrors PowerSensor

Our Power Sensor is an affordable piece of kit to check your infrared CO2 lasertube and keep track of it's power over time. 
At the moment we supply it as the Sensor only, you will have to hook it up to an Arduino flash our arduino sketch. 
The calibration data is stored on the probe and writteh on the sleeve of it's cable. 

## Contents:

- PowerSensor calibrated and labeled with it's id and calibration data. 
- Matching  JST SM 2,54mm plug with wires
- 4,7k Ohm Resistor 

## Specs

- usable for 20-150W infrared CO2 laser tubes.
- asolue accuracy: ±5W
- relative rerror: <±2W

## Therory of operation

To measure the power of a laser tube you fire the laser at a target of 
known thermal capacity for a know amount of time and measure the temperature increase that causes.
The power probe consits of a block of black anodized aluminium with a digital temperature sensor in it. 
The black anodizing absorbs the infrared laser beam and transfers the heat to the aluminum block. 
A 1-wire temperature sensor inside the block measures the rise in temperature while the laser is firing. 


## Wiring 

The JST SM receptical attached to the power seonsor only has 3 pins in a 4 wire housing for futre expansion. 

###Pinout 


| Pin | color on sensor | color on plug | Arduino Pin |
|-----|-----------------|---------------|-------------|
| 1   | yellow          | black         | A0          |
| 2   |                 | green         |             |
| 3   | red             | red           | 5V/3,3V     |
| 4   | black           | blue          | GND         |

![](/img/Cable.png)

### Connecting to Arduino

Connect the probe as follows:

blue wire to any GND pin
red wire to 5/3.3V
black to A0 

Now add a 4,7k Ohm pullup resistor between the red and black wire. 

The green wire remains unconnected. 

## Software 

To write the software to your Arduino you will need the Arduino IDE:

https://www.arduino.cc/en/Main/Software

And the "DallasTemperature" library which can be installed via the library manager:
https://www.arduino.cc/en/Guide/Libraries

Download this repository and flash the sketch to your arduino
https://github.com/GitNMirrors/PowerSensor/archive/master.zip


## Measure Power

You will have to set your laser controller up to fire at full power for 20s. 

*During these 20s the power probe will get hot! Especially with lasers above 50W it's temperature will exceed 60°C!*

Some controllers can only be set to fire for 10s (Ruida) it is okay to fire those for a second 9999ms shortly after the first. 

Connect your Arduino with the PowerSensor to your computer and in the Arduino IDE open the Serial Monitor at 115200 Baud.

Mount the sensor block on the stainless tube sticking out of the black block or suspend the sensor on it's cable just in front of the laser tube. 
Follow the intructions in the serial terminal and fire tle laser for 20s. 

Keep in mind, that the block stays hot afte the mesurement for a while.








