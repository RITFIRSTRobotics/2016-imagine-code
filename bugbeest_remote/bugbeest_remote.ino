#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <SPI.h>
#include <math.h>

//#include <RH_NRF24.h>
//#include <RHDatagram.h>


#define BB_X 14
#define BB_Y 15


#define LED_DIN 6
#define BUTTON_A 4
#define BUTTON_B 3
#define BUTTON_Z 7

// GREG WAS HERE
// Change numbers
#define BTN_1 1   // Joystick Button Press
#define BTN_2 2   // Left Button
#define BTN_3 3   // Right Button
#define BTN_4 4   // Center Button
#define BTN_CHNL 5   // Channel Button
// GREG HAS GONE

#define BB_X_AZ 10 //active zone of the remote's x axis in the y direction. 
                   //when abs(bb_y-128) is less than this, x axis is read.
#define BB_XY_DZ 5 

#define REMOTE_ADDR 111

#define VERBOSE 1

byte bb_x,bb_y;
bool brake = 1, ch_change, open_wings;
int RF_buffer[4];
byte BB_ADDR = 101;
bool prev_state_A;

// GREG WAS HERE
int robot_type;
int control_scheme;
int btn_1_state = 0;
int btn_2_state = 0;
int btn_3_state = 0;
int btn_4_state = 0;
int btn_chnl_state = 0;
// GREG HAS GONE

//RH_NRF24 driver;
//RHDatagram manager(driver, REMOTE_ADDR);
RF24 radio(8,10);
Adafruit_NeoPixel ch_led= Adafruit_NeoPixel(1, LED_DIN, NEO_RGB + NEO_KHZ800);
void setup() {
  pinMode(BB_X, INPUT);
  pinMode(BB_Y, INPUT);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_Z, INPUT);
  // GREG WAS HERE
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  pinMode(BTN_4, INPUT);
  pinMode(BTN_CHNL, INPUT);
  control_scheme = 0;
  // GREG HAS GONE
  
  Serial.begin(19200);
   radio.begin();
   radio.openWritingPipe(REMOTE_ADDR);
//  if (!manager.init())
//    Serial.println("failed init of radio");
//    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  //radio.stopListening();
  ch_led.begin();
  
}
void loop() {
  ch_led.setPixelColor(0, ch_led.Color(50,50,50));
  ch_led.show();
  bb_x = map(analogRead(BB_X),0,1024,0,255);
  bb_y = map(analogRead(BB_Y),0,1024,0,255);

  //ch_change = digitalRead(BUTTON_B);
  if (digitalRead(BUTTON_A) == 0 && prev_state_A == 1){
    brake = !brake;
    prev_state_A = 0;
  }
  else prev_state_A = 1;
  open_wings = digitalRead(BUTTON_Z);

  // GREG WAS HERE

  // numbers need changing as well
  btn_chnl_state = digitalRead(BTN_CHNL);
  if (btn_chnl_state == 1){
    robot_type = get_robot_type();
    switch (robot_type){
      case 1:
        // Base Robot
        control_scheme = 1;
        break;
      case 2:
        // Solenoid Robot
        control_scheme = 2;
        break;
      case 3:
        // Pincher Robot
        control_scheme = 3;
        break;
    }
  }

  if (control_scheme == 0){
    
  }
  
  // GREG HAS GONE
  
/*
 * Order of data in buffer:
 * 
 * [bb_x,bb_y,bb_z, brake_mode]
 * 
 * bb_x: the joystyick position in the x axis, -127 to 127, 0 is neutral
 * bb_y: joystick position in y axis, same as above
 * bb_z: push button on joystick, boolean, will be used to open wings eventually
 * brake_mode: button to toggle motor braking, boolean
 */
  RF_buffer[0] = constrain(bb_x,0,254)-127;
  RF_buffer[1] = constrain(bb_y,0,254)-127;
  RF_buffer[2] = open_wings;
  RF_buffer[3] = brake;

    // // if ((abs(bb_x-128)>BB_XY_DZ) || (abs(bb_y-128)>BB_XY_DZ)){
    //    manager.sendto( RF_buffer, sizeof(RF_buffer), BB_ADDR);
  radio.write(&RF_buffer, sizeof(RF_buffer));
  if (VERBOSE){
    if (Serial){
      Serial.print("X: ");
      Serial. print (bb_x);
      Serial.print("Y: ");
      Serial. print (bb_y);
      Serial.print("brake: ");
      Serial. println (brake);
    }
  }
}
