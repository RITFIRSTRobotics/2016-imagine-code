//#include <nRF24L01.h>
//#include <RF24.h>
//#include <RF24_config.h>
/*
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
*/
#include <SPI.h>
#include <math.h>

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
//#include <RHDatagram.h>


#define BB_X 14
#define BB_Y 15


#define LED_DIN 6
/**
#define BUTTON_A 4
#define BUTTON_B 3
#define BUTTON_Z 7
*/

// Need to change numbers
#define BTN_1 1   // Joystick Button Press
#define BTN_2 2   // Left Button
#define BTN_3 3   // Right Button
#define BTN_4 4   // Center Button
#define BTN_CHNL 5   // Channel Button

#define BB_X_AZ 10 //active zone of the remote's x axis in the y direction. 
                   //when abs(bb_y-128) is less than this, x axis is read.
#define BB_XY_DZ 5 

#define REMOTE_ADDR 111

#define VERBOSE 1

byte bb_x,bb_y;
uint8_t RF_buffer[8];
byte BB_ADDR = 101;
//bool prev_state_A;

int robot_type;         // variable that stores current type of robot
int control_scheme;     // variable that stores current control scheme of remote
int btn_1_state = 0;    // on/off states for the buttons, might not be needed
int btn_2_state = 0;
int btn_3_state = 0;
int btn_4_state = 0;
int btn_chnl_state = 0; 
bool pick_up, fire;
bool open_claw, close_claw;
bool raise_arm, lower_arm;
uint8_t type_buffer[1];
uint8_t recv_buffer[RH_NRF24_MAX_MESSAGE_LEN];

RH_NRF24 driver;
RHReliableDatagram manager(driver, REMOTE_ADDR);
//RF24 radio(8,10);
//Adafruit_NeoPixel ch_led= Adafruit_NeoPixel(1, LED_DIN, NEO_RGB + NEO_KHZ800);
void setup() {
  pinMode(BB_X, INPUT);
  pinMode(BB_Y, INPUT);
  /**
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_Z, INPUT);
  */
  // GREG WAS HERE
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  pinMode(BTN_4, INPUT);
  pinMode(BTN_CHNL, INPUT);
  control_scheme = 0;         // default control scheme
  // GREG HAS GONE
  
  Serial.begin(19200);
   // radio.begin();
   // radio.openWritingPipe(REMOTE_ADDR);
  if (!manager.init()){
    Serial.println("failed init");
    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  }
  //radio.stopListening();
  //ch_led.begin();
}

void loop() {
  //ch_led.setPixelColor(0, ch_led.Color(50,50,50));
  //ch_led.show();
  bb_x = map(analogRead(BB_X),0,1024,0,255);
  bb_y = map(analogRead(BB_Y),0,1024,0,255);

  //ch_change = digitalRead(BUTTON_B);
  /**
  if (digitalRead(BUTTON_A) == 0 && prev_state_A == 1){
    brake = !brake;
    prev_state_A = 0;
  }
  else prev_state_A = 1;
  open_wings = digitalRead(BUTTON_Z);
  */
  
  // numbers need changing as well
  if (digitalRead(BTN_CHNL) == 1){
    type_buffer[0] = '*';
    uint8_t len = sizeof(recv_buffer);
    uint8_t from;
    if (!manager.sendtoWait( type_buffer, sizeof(type_buffer), BB_ADDR)){
      Serial.println("sendtoWait for type_buffer failed");
    }
    if (manager.recvfromAck(recv_buffer, &len, &from)){
      robot_type = (int)recv_buffer[0];
    }
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
    // do nothing?
    // 0 and 1 might end up being the same
  }

  if (control_scheme == 1) {
    // movement only, disable buttons (except change channel)
  }

  if (control_scheme == 2) {
    // movement + buttons for pick up and fire
    if (digitalRead(BTN_4) == 1 && pick_up == 0){
      pick_up = 1;
    }
    if (digitalRead(BTN_1) == 1 && fire == 0){
      fire = 1;
    }
  }

  if (control_scheme == 3) {
    // movement + buttons for open/close, raise/lower
    if (digitalRead(BTN_1) == 1 && open_claw == 0){
      open_claw = 1;
      close_claw = 1;
    }
    if (digitalRead(BTN_4) == 1 && close_claw == 0){
      close_claw = 1;
      open_claw = 0;
    }
    if (digitalRead(BTN_3) == 1 && raise_arm == 0){
      raise_arm = 1;
      lower_arm = 0;
    }
    if (digitalRead(BTN_2) == 1 && lower_arm == 0){
      lower_arm = 1;
      raise_arm = 0;
    }
  }
  
/*
 * Order of data in buffer:
 * [bb_x,bb_y,bb_z, pick_up, fire, open_claw, close_claw, raise_arm, lower_arm]
 * 
 * Ben's old buffer:
 * [bb_x,bb_y,bb_z, brake_mode]
 * 
 * bb_x: the joystyick position in the x axis, -127 to 127, 0 is neutral
 * bb_y: joystick position in y axis, same as above
 * bb_z: push button on joystick, boolean, will be used to open wings eventually
 * brake_mode: button to toggle motor braking, boolean
 */

/**
  RF_buffer[0] = constrain(bb_x,0,254)-127;
  RF_buffer[1] = constrain(bb_y,0,254)-127;
  RF_buffer[2] = open_wings;
  RF_buffer[3] = brake;
*/
  RF_buffer[0] = constrain(bb_x,0,254)-127;
  RF_buffer[1] = constrain(bb_y,0,254)-127;
  RF_buffer[2] = pick_up;
  RF_buffer[3] = fire;
  RF_buffer[4] = open_claw;
  RF_buffer[5] = close_claw;
  RF_buffer[6] = raise_arm;
  RF_buffer[7] = lower_arm;

    // // if ((abs(bb_x-128)>BB_XY_DZ) || (abs(bb_y-128)>BB_XY_DZ)){
  if (!manager.sendtoWait( RF_buffer, sizeof(RF_buffer), BB_ADDR)){
    Serial.println("sendtoWait for RF_buffer failed");
  }
  // radio.write(&RF_buffer, sizeof(RF_buffer));
  if (VERBOSE){
    if (Serial){
      Serial.print("X: ");
      Serial. print (bb_x);
      Serial.print("Y: ");
      Serial. print (bb_y);
    }
  }
}
