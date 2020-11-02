# RC Car with Camera Streaming

This repository was developed for Case Western Reserve University's ECSE 398 senior project course. Contributers to this project are Juliette Naugle, Michael Heeschen, and Avery Gould. The project is to build an RC car that transmits signals, drives and steers, and camera streams over wifi. This reposity provides code for 2 Arduino Pro Minis and a Raspberry Pi 3B that al. C++ and Python are both used in the code files.


## Microcontrollers:


- **Arduino Pro Mini in the car:** used for driving and steering functionality and receiving signals from the Arduino in the controller
- **Arduino Pro Mini in the controller:** used for sending signals to the Arduino in the car based on the inputs from the controller
- **Raspberry Pi 3B in the car:** used primarily for camera streaming


## Prerequisites:


- **Arduino in the controller:** C++ libraries SPI.h and RH_ASK.h
- **Arduino in the car:** C++ libraries SPI.h and RH_ASK.h
- **Raspberry Pi:** Python packages RPi.GPIO, gpiozero, time, and os


## Microcontroller pinouts:


### Arduino in the controller:

- Pin 17: connected to the button pin on the joystick
- Pin 5: connected to button 1
- Pin 6: connected to button 2
- Pin 3: connected to switch 1
- Pin 4: connected to switch 2
- Pin A0: connected to joystick pin for x direction
- Pin A2: connected to joystick pin for y direction
- Pin A3: connected to data pin of transmitter


### Arduino in the Car:


- Pin 10: connected enable A on motor driver 1
- Pin 12: connected enable B on motor driver 1
- Pin 9: connected input 1 on motor driver 1
- Pin 8: connected input 2 on motor driver 1
- Pin 7: connected input 3 on motor driver 1
- Pin 6: connected input 4 on motor driver 1
- Pin 13: connected enable A on motor driver 2
- Pin 11: connected enable B on motor driver 2
- Pin 5: connected to input 1 on motor driver 2
- Pin 4: connected to input 2 on motor driver 2
- Pin 3: connected to input 3 on motor driver 2
- Pin 2: connected to input 4 on motor driver 2
- Pin A1: connected to data pin of receiver


### Raspberry Pi in the Car:


## Files and descriptions:


**arduino_car_4wheel_steer:** contains code used for the arduino in the car

**arduino_controller:** contains code used for the arduino in the controller

**rpi:** contains code used for the raspberry pi in the car
