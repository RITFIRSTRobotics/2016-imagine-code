#include <Servo.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

/*
 * MyClass.h
 *
 * Created: 2/22/2016 12:26:27 AM
 * Author: ben parnas
 */ 

#ifndef IMAGINEBOTS_V2_H	
#define IMAGINEBOTS_V2_H	

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>
//#include <Servo/src/Servo.h>
#include <SPI.h>
#include <math.h>
#include "ImagineBotsPinDefs.h"
#include "ImagineBotsConstants.h"


enum botType{
	PLOWBOT = 0,
	ARMBOT,
	SHOOTBOT
};

class imagineBots_motor
{
	private:
	uint8_t speed;
	boolean dir;
	uint8_t directionPin;
	uint8_t enablePin;
	
	public:
	imagineBots_motor(uint8_t __enablePin, uint8_t __directionPin );
	void setSpeed(int);
	void update(void);
	
	
	
};

class ImagineBots_v2
{
 private:
	char botData[5];
		
	botType robot_type;
	
	uint64_t botAddress;//internal address of the robot
	uint8_t commAddress;//address of the remote controlling it
	boolean data_available;//set if interrupt has run and there is data available. 
	uint8_t LEDR,LEDB,LEDG;
	
	
 public:
	boolean requestBotType;
	uint8_t xVal;
	uint8_t yVal;
	uint8_t armPos;
	uint8_t grabPos;
	uint8_t fireSolenoid;
	
	
	ImagineBots_v2(botType robot_type, char* botAddress);
	imagineBots_motor lMotor;
	imagineBots_motor rMotor; //drive motors
	Adafruit_NeoPixel CH_led; //LED control object
	RF24 radio;
	//RF24Network dataManager;
	Servo armServo;//a servo that controls a two-position arm. used on both armbot and shootbot
	Servo clawServo;//a servo that controls a grabber. used on armbot
	
	
	
	
	
	
	void setupPins();
	boolean setupComms();
	
	//set a flag that data is available, called on pin interrupt from NRF24L01
	void setDataAvaialble(boolean val);
	
	//read data from NRF24L01, store it in an array
	void readComms(void);
	
	//returns the robot address
	uint8_t getAddress();
	
	//set the robot's address
	void setAddress(uint8_t address);
	
	//disables the robot's motors and servos
	void disableBot();
	
	//enables the robot's motors and servos
	void enableBot();
	
	//send the robot type to the provided address
	boolean sendRobotType();
	
	void updateLED();

 void testMotors();
	
	
	
	
	
};



#endif

