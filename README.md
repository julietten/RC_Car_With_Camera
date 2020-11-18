# RC Car with Camera Streaming

This repository was developed for Case Western Reserve University's ECSE 398 senior project course. Contributers to this project are Juliette Naugle, Michael Heeschen, and Avery Gould. The project is to build an RC car that transmits signals, drives and steers, and camera streams over wifi. This repository provides code for two Arduino Pro Minis and a Raspberry Pi 3B along with a user-friendly Flask application to help instruct the user on setting up their own RC car and document their observations while streaming. C++, Python, and HTML with internal CSS are used in the code files.

## Contents:

1. Microcontrollers
2. Prerequisites
3. Files and Descriptions
4. Hardware Components
  a. Controller
  b. Car
5. Microcontroller Pinouts
  a. Arduino in the Controller
  b. Arduino in the Car
  c. Raspberry Pi in the Car

## 1. Microcontrollers:

- **Arduino Pro Mini in the car:** used for driving and steering functionality and receiving signals from the Arduino in the controller
- **Arduino Pro Mini in the controller:** used for sending signals to the Arduino in the car based on the inputs from the controller
- **Raspberry Pi 3B in the car:** used primarily for camera streaming

## 2. Prerequisites:

- **Arduino in the controller:** C++ libraries SPI.h and RH_ASK.h
- **Arduino in the car:** C++ libraries SPI.h and RH_ASK.h
- **Raspberry Pi:** Python packages time and os
- **Flask Application:** Python packages flask, paramiko, os, and datetime

## 3. Files and Descriptions:

**arduino_car_4wheel_steer.cpp:** contains C++ code used for the Arduino in the car

**arduino_controller.cpp:** contains C++ code used for the Arduino in the controller

**rpi.py:** contains Python code used for the Raspberry Pi in the car

**flask folder contains:** 
- stream_webpage.py: user-friendly Flask application Python file
- templates folder: contains the setup.html and streaming.html templates for the Flask application

## 4. Hardware Components:
(refer to the Schematics folder to see the connections)

### Controller:
- Arduino Pro Mini
- 433 MHz transmitter (in a transmitter and receiver pair)
- joystick
- 2x push buttons
- 2x switches

### Car:
- Arduino Pro Mini
- Raspberry Pi
- 433 MHz receiver (in a transmitter and receiver pair)
- 2x L298N motor drivers
- 4x motors
- Raspberry Pi camera module

## 5. Microcontroller Pinouts:

### Arduino in the Controller:

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
-Attach the camera module to the camera port
