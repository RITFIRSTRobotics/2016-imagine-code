/*
 * MyClass.cpp
 *
 * Created: 2/22/2016 12:26:27 AM
 * Author: benja
 */ 

#include "ImagineBots_v2.h"
#include <stdio.h>

ImagineBots_v2::ImagineBots_v2(botType __robot_type, char * __botAddress):radio(TRX_CE,TRX_CSN),CH_led(2,LED_DIN,NEO_RGB+NEO_KHZ800),
lMotor(LMOTOR_VEL, LMOTOR_DIR),rMotor(RMOTOR_VEL, RMOTOR_DIR){
	robot_type = __robot_type;
	botAddress = atoi(__botAddress);
	
	
	if (robot_type == ARMBOT){
		armServo.attach(SERVO_ONE);
		clawServo.attach(SERVO_TWO);
	}
	else if(robot_type == SHOOTBOT){
		armServo.attach(SERVO_ONE);
	}
	
}

void ImagineBots_v2::setupPins(){
	pinMode(TRX_IRQ,INPUT_PULLUP);
	pinMode(LMOTOR_VEL,OUTPUT);
	pinMode(RMOTOR_VEL,OUTPUT);
	pinMode(LMOTOR_DIR,OUTPUT);
	pinMode(RMOTOR_DIR,OUTPUT);
	pinMode(LED_DIN, OUTPUT);
	pinMode(SERVO_ONE,OUTPUT);
	pinMode(SERVO_TWO,OUTPUT);
	pinMode(SERVO_THREE, OUTPUT);
	pinMode(SERVO_FOUR, OUTPUT);
	pinMode(CHANGE_CH, OUTPUT);
}
boolean ImagineBots_v2::setupComms(){
	boolean success = true;
	radio.begin();
	radio.openReadingPipe(1,botAddress);
	radio.openReadingPipe(2, atoi(FIELD_ADDRESS_RF24));
	return success;
}

void ImagineBots_v2::setDataAvaialble(boolean val){
	data_available = val;
}

//will be called if data_available flag has been set
void ImagineBots_v2::readComms(){
	while (radio.available()){
		radio.read(&botData, sizeof(botData));
	}
	
}

void ImagineBots_v2::setAddress(uint8_t address){	
	botAddress = address;
}

uint8_t ImagineBots_v2::getAddress(){
	return botAddress;
}
//enable the motors on the bot, reset their values. 
void ImagineBots_v2::enableBot(){
	digitalWrite(MOTOR_EN, 1);
	lMotor.setSpeed(0);
	lMotor.update();
	rMotor.setSpeed(0);
	rMotor.update();
	
	if(robot_type == ARMBOT)
	{
		armServo.write(ARMBOT_ARM_LOW);
		clawServo.write(ARMBOT_CLAW_OPEN);
	}
	else if(robot_type == SHOOTBOT){
		armServo.write(SHOOTBOT_PICKUP_LOW);
	}
	
	
}

void ImagineBots_v2::disableBot(){
	digitalWrite(MOTOR_EN,0);
}

void ImagineBots_v2::updateLED(){
	if (botAddress == atoi(ROBOT_ADDRESS_1_RF24)){
		LEDR = LED_MAGENTA_R;
		LEDG = LED_MAGENTA_G;	
		LEDB = LED_MAGENTA_B;
	}
	
	else if(botAddress == atoi(ROBOT_ADDRESS_2_RF24)){
		LEDR = LED_CYAN_R;
		LEDG = LED_CYAN_G;
		LEDB = LED_CYAN_B;
	}
	else if(botAddress == atoi(ROBOT_ADDRESS_3_RF24)){
		LEDR = LED_YELLOW_R;
		LEDG = LED_YELLOW_G;
		LEDB = LED_YELLOW_B;
	}
	else if(botAddress ==  atoi(ROBOT_ADDRESS_4_RF24)){
		LEDR = LED_GREEN_R;
		LEDG = LED_GREEN_G;
		LEDB = LED_GREEN_B;
	}
	
	CH_led.setPixelColor(0,CH_led.Color(LEDR,LEDG,LEDB));
}

void ImagineBots_v2::testMotors() {
  digitalWrite(MOTOR_EN, 1);
  lMotor.setSpeed(500);
  lMotor.update();
  rMotor.setSpeed(500);
  rMotor.update();
}

imagineBots_motor::imagineBots_motor(uint8_t __enablePin, uint8_t __directionPin ){
	directionPin = __directionPin;
	enablePin = __enablePin;
	speed = 0;
	dir = 0;
	
	
}

void imagineBots_motor::setSpeed( int velocity){
	if(velocity < 0){
		speed = abs(velocity);
		dir = 1;
	}
	else{
		speed = velocity;
		dir = 0;
	}
}

void imagineBots_motor::update(){
	analogWrite(enablePin,speed);
	digitalWrite(directionPin,dir);
}
