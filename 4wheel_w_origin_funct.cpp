#include <SPI.h>
#include <RH_ASK.h>

//defines arduino pins - without b are front motors with b are back motors
#define enA 10 //enable for motor A
#define enB 12 //enable for motor B
#define in1 9 //backward movement input of motor A
#define in2 8 //forward movement input of motor A
#define in3 7 //forward movement input of motor B
#define in4 6 //backward movement input of motor B
#define enAb 13 //enable for motor C
#define enBb 11 //enable for motor D
#define in1b 5 //backward movement input of motor C
#define in2b 4 //forward movement input of motor C
#define in3b 3 //forward movement input of motor D
#define in4b 2 //backward movement input of motor D

RH_ASK driver(2000,A3,11,A1,false); //initializes radio/transmitter object
unsigned long lastReceiveTime = 0; //stores the time when the last signal sent was received
unsigned long currentTime = 0; //stores the current time

//vars for each button and joystick movement in the signal
uint8_t jpotx = 0; //stores analog input x value of joystick
uint8_t jpoty = 0; //stores analog input y value of joystick
uint8_t jbutton = 0; //stores digital input of joystick button
uint8_t button1 = 0; //stores digital input of button 1
uint8_t button2 = 0; //stores digital input of button 2
uint8_t switch1 = 0; //stores digital input of switch 1
uint8_t switch2 = 0; //stores digital input of switch 2

//vars related to driving and steering
int steering, throttle; //stores steering and throttle values respectively
int motorSpeedA = 0; //stores the speed of motor A
int motorSpeedB = 0; //stores the speed of motor B
int motorSpeedC = 0; //stores the speed of motor C
int motorSpeedD = 0; //stores the speed of motor D
int lowTrigger = 100; //stores value that determines when a motor moves backward
int highTrigger = 180; //stores value that determines when a motor moves forward
int pwmVal = 70; //stores pwm value - potentially a dummy val but could work

//vars related to the return to origin switch
int max = 100; //max array size
uint8_t* signals = new uint8_t[max]; //stores signals received
unsigned long* times = new unsigned long[max]; //stores times between signals
int n = 0; //stores number of signals received
int m = 1; //flags if signal backtracked -- 180 degree turn only if first
uint8_t revSignal[7] = 0; //stores signal being backtracked
uint8_t revTime = 0; //stores time of signal being backtracked

void defaultData(){ //function that stores neutral values of each input
	jpotx = 127; //sets x movement of joystick to neutral, values range from 0 to 255
	jpoty = 127; //sets y movement of joystick to neutral, values range from 0 to 255
	jbutton = 1; //sets joystick button to neutral, 1 because it has pullup resistors set in controller
	button1 = 1; //sets button 1 to neutral, 1 because it has pullup resistors set in controller
	button2 = 1; //sets button 2 to neutral, 1 because it has pullup resistors set in controller
	switch1 = 1; //sets switch 1 to neutral, 1 because it has pullup resistors set in controller
	switch2 = 1; //sets switch 2 to neutral, 1 because it has pullup resistors set in controller
}

void setup(){ //function that sets up motor connection, receiver, and sets default values to all inputs
	//lines 52-63 set all motor connections as outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(enAb, OUTPUT);
	pinMode(enBb, OUTPUT);
	pinMode(in1b, OUTPUT);
	pinMode(in2b, OUTPUT);
	pinMode(in3b, OUTPUT);
	pinMode(in4b, OUTPUT);
	driver.init(); //initializes transmitter and receiver object to prepare to receive signals
	defaultData(); //sets all input button values to default
	Serial.begin(9600);  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop(){

	currentTime = millis(); //saves the current time
	if (currentTime - lastReceiveTime > 1000){ //determines if there is a lost connection
		defaultData(); //resets data to default if connection lost
	}

	//defines signal as array of bits and divides by each byte's function
	uint8_t data[7]; //stores the signal where each item in array is a byte received
	uint8_t dataLength = 7; //stores amount of bytes in the signal received
	if (driver.recv(data, &dataLength)){ //receives the signal from transmitter
		jpotx = data[0]; //stores first byte received
		jpoty = data[1]; //stores second byte received
		jbutton = data[2]; //stores third byte received
		button1 = data[3]; //stores fourth byte received
		button2 = data[4]; //stores fifth byte received
		switch1 = data[5]; //stores sixth byte received
		switch2 = data[6]; //stores seventh byte received
    	lastReceiveTime = millis(); //stores the last receive time
    	int i = 0; //stores loop index
    	for (i=0; i<dataLength; i++){ //loops through bytes in the signal
    		Serial.println(data[i], BIN); //prints binary values of each input
    	}
    }

    signals[n] = data; //stores the most recent signal //declare after signal received - line 69
    times[n] = currentTime - lastReceiveTime; //stores the time between signals //declare before new receive time on line 76

    steering = jpotx; //stores x value of the joystick
    throttle = jpoty; //stores y value of the joystick

    if (switch1 == 0 && n > 0) {//determines if switch one is on and if there are signals to backtrack
        if (m == 1){ //switch just turned on

            //180 degree turn, 200 set for A and C makes turn rightward
            motorSpeedA = 200;
            motorSpeedB = 0;
            motorSpeedC = 200;
            motorSpeedD = 0;
            analogWrite(enA, motorSpeedA);
            analogWrite(enB, motorSpeedB);
            analogWrite(enAb, motorSpeedC);
            analogWrite(enBb, motorSpeedD);
            delay(1000); //gives car 1 second to turn with motors A and C set to turn
            //stops turning after one second, all motors set to stopped
            motorSpeedA = 0;
            motorSpeedB = 0;
            motorSpeedC = 0;
            motorSpeedD = 0;
            analogWrite(enA, motorSpeedA);
            analogWrite(enB, motorSpeedB);
            analogWrite(enAb, motorSpeedC);
            analogWrite(enBb, motorSpeedD);
        }
        m=0; //flags that car has done 180 degree turn
        revSignal = signals[n]; //stores next backtracking signal
        steering = revSignal[0]; //stores y movement of joystick from next backtracking signal
        throttle = revSignal[1]; //stores x movement of joystick from next backtracking signal
        revTime = times[n]; //stores time of signal being backtracked
    }

    if (throttle < lowTrigger){ //sets motors A B C D to go backwards
    	digitalWrite(in1, HIGH);
    	digitalWrite(in2, LOW);
    	digitalWrite(in3, LOW);
    	digitalWrite(in4, HIGH);
    	digitalWrite(in1b, HIGH);
    	digitalWrite(in2b, LOW);
    	digitalWrite(in3b, LOW);
    	digitalWrite(in4b, HIGH);
    	//maps throttle reading to PWM signal
    	motorSpeedA = map(throttle, lowTrigger, 0, 0, 255);
    	motorSpeedB = map(throttle, lowTrigger, 0, 0, 255);
    	motorSpeedC = map(throttle, lowTrigger, 0, 0, 255);
    	motorSpeedD = map(throttle, lowTrigger, 0, 0, 255);
    }
    if (throttle > highTrigger){ //sets motors A B C D to go forwards
    	digitalWrite(in1, LOW);
    	digitalWrite(in2, HIGH);
    	digitalWrite(in3, HIGH);
    	digitalWrite(in4, LOW);
    	digitalWrite(in1b, LOW);
    	digitalWrite(in2b, HIGH);
    	digitalWrite(in3b, HIGH);
    	digitalWrite(in4b, LOW);
    	//maps throttle reading to PWM signal
    	motorSpeedA = map(throttle, highTrigger, 255, 0, 255);
    	motorSpeedB = map(throttle, highTrigger, 255, 0, 255);
    	motorSpeedC = map(throttle, highTrigger, 255, 0, 255);
    	motorSpeedD = map(throttle, highTrigger, 255, 0, 255);
    }
    if (throttle >= lowTrigger && throttle <= highTrigger){ //cars stops if throttle within range of low and high trigger
    	motorSpeedA = 0; //sets motor A as stopped
    	motorSpeedB = 0; //sets motor B as stopped
    	motorSpeedC = 0; //sets motor C as stopped
    	motorSpeedD = 0; //sets motor D as stopped
    }
    if (steering > highTrigger){ //determines motor speeds for steering left
    	int xMapped = map(steering, highTrigger, 255, 0, 255); //stores steering value
    	motorSpeedA = motorSpeedA - xMapped; //motor speed reduced due to left turn
    	if (motorSpeedA < 0){ //motor speed less than 0 cannot be stored, sets to 0
    		motorSpeedA = 0;
    	}
    	motorSpeedC = motorSpeedC - xMapped; //motor speed reduced due to left turn
    	if (motorSpeedC < 0){ //motor speed less than 0 cannot be stored, sets to 0
    		motorSpeedC = 0;
    	}
    	motorSpeedB = motorSpeedB + xMapped; //motor speed increased due to left turn
    	if (motorSpeedB > 255){ //motor speed more than 255 cannot be stored, sets to 255
    		motorSpeedB = 255;
    	}
    	motorSpeedD = motorSpeedD + xMapped; //motor speed increased due to left turn
    	if (motorSpeedD > 255){ //motor speed more than 255 cannot be stored, sets to 255
    		motorSpeedD = 255;
    	}
    }
    if (steering < lowTrigger){ //determines motor speeds for steering right
    	int xMapped = map(steering, lowTrigger, 0, 0, 255); //stores steering value
    	motorSpeedA = motorSpeedA + xMapped; //motor speed increased due to right turn
    	if (motorSpeedA > 255){ //motor speed more than 255 cannot be stored, sets to 255
    		motorSpeedA = 255;
    	}
    	motorSpeedC = motorSpeedC + xMapped; //motor speed increased due to right turn
    	if (motorSpeedC > 255){ //motor speed more than 255 cannot be stored, sets to 255
    		motorSpeedC = 255;
    	}
    	motorSpeedB = motorSpeedB - xMapped; //motor speed reduced due to right turn
    	if (motorSpeedB < 0){ //motor speed less than 0 cannot be stored, sets to 0
    		motorSpeedB = 0;
    	}
    	motorSpeedD = motorSpeedD - xMapped; //motor speed reduced due to right turn
    	if (motorSpeedD < 0){ //motor speed less than 0 cannot be stored, sets to 0
    		motorSpeedD = 0;
    	}
    }
    if (motorSpeedA < pwmVal){ //prevents buzzing when speed is low
    	motorSpeedA = 0; //sets speed of motor A to 0
    }
    if (motorSpeedB < pwmVal){ //prevents buzzing when speed is low
    	motorSpeedB = 0; //sets speed of motor A to 0
    }
    if (motorSpeedC < pwmVal){ //prevents buzzing when speed is low
    	motorSpeedC = 0; //sets speed of motor A to 0
    }
    if (motorSpeedD < pwmVal){ //prevents buzzing when speed is low
    	motorSpeedD = 0; //sets speed of motor A to 0
    }
    analogWrite(enA, motorSpeedA); //determines speed control for motor A by sending PWM signal
    analogWrite(enB, motorSpeedB); //determines speed control for motor B by sending PWM signal
    analogWrite(enAb, motorSpeedC); //determines speed control for motor C by sending PWM signal
    analogWrite(enBb, motorSpeedD); //determines speed control for motor D by sending PWM signal

    if (switch1 == 0 && n > -1){ //switch on and backtrack index exists, delays time so movement matches
        delay(revTime); //delays time based on backtracked signal time
        signals[n] = 0; //removes signal thats been backtracked
        times[n] = 0;  //removes time taken for signal thats been backtracked
        n = n - 1; //decrements elements in array so that signals added in order
    } else if (n < 0){ //cant backtrack anymore, wait to see if new signal received
        delay(100); //short wait
    } else { //if switch is off
        n = n + 1; //increments n so signals&times arent overwritten
        m = 1; //flag set to 1 since car is not backtracking, allows for the 180 degree turn if switch one is turned on
    }

    if (n == max - 1 || n < max - 100){ //dynamic allocation for signals and times arrays
        if (n >= max){ //if n goes beyond the size of the arrays, increase their sizes
            max = max + 100; //increases size of arrays
        }
        if (n < max - 100){ //if n goes below the size of the array, decrease their sizes
            max = max - 100; //decreases size of arrays
        }
        uint8_t* temps = new uint8_t[max]; //creates signals array in temporary array size of the new max value
        unsigned long* tempt = new unsigned[max]; //creates times array in temporary array size of the new max value
        for (int i = 0; i < n; i++){ //loops through each of the original signals and times arrays
            temps[i] = signals[i]; //adds signals to the temporary signals array
            tempt[i] = times[i]; //adds times to the temporary times array
        }
        delete [] signals; //deletes stored values in the signals array
        delete [] times; //deletes stored values in the times array
        signals = temps; //stores the signals temp array in the signals arrays
        times = tempt; //stores the times temp array in the times array
    }
}