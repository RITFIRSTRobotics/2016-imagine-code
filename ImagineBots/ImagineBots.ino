#include <SoftwareServo.h>
#include <SPI.h>
//#include <RHDatagram.h>
//#include <RH_NRF24.h>

/*
 * ImagineBots_v2.ino
 *
 * Created: 2/22/2016 12:26:27 AM
 * Author: benja
 */ 

#include "ImagineBots_v2.h"


botType bot_type = PLOWBOT;
char address[] = ROBOT_ADDRESS_1_RF24;
ImagineBots_v2 bot(bot_type, address);

void setup()
{
	
	Serial.begin(9600);
	SPI.begin();
	bot.setupPins();
	if(~(bot.setupComms())){
		if(Serial){
			Serial.println("did not set up radio");
		}
	}
	//attachInterrupt(TRX_IRQ,LOW);
	
	bot.CH_led.begin();
 Serial.println("setup done!");
}

void loop()
{
	//assume that the values in the array are up to date, as the interrupt is keeping them new.
	//[x,y,arm/loader position, claw  position/fire,request_bot_type]

 bot.testMotors();
 bot.updateLED();
 delay(20);
	
}
