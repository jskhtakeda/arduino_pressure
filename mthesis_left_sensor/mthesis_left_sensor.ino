#include <ros.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int16MultiArray.h"


ros::NodeHandle nh;
std_msgs::Int16MultiArray AP_msg_L;
std_msgs::Int16MultiArray AP_msg_L_Dif;
ros::Publisher pub_temp("AirPressure_Left", &AP_msg_L);
ros::Publisher pub_temp3("AirPressure_Left_Dif", &AP_msg_L_Dif);

#define DATAPIN_SENSOR 2
#define LATCHPIN_SENSOR 4
#define CLOCKPIN_SENSOR 6

const uint8_t PATTERNS[] = {
  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001,
  0b00000000
};

int16_t vals[48]={};
int16_t first_vals[48]={};
int16_t dif_vals[48]={};
int vals_sum = 0;

void setup() {
  
  nh.initNode();
  AP_msg_L.data_length = 48;
  AP_msg_L.data = (int16_t *)malloc(sizeof(int16_t)*48);  
  AP_msg_L_Dif.data_length = 48;
  AP_msg_L_Dif.data = (int16_t *)malloc(sizeof(int16_t)*48);  

  nh.advertise(pub_temp);
  nh.advertise(pub_temp3);

  pinMode(DATAPIN_SENSOR, OUTPUT);
  pinMode(LATCHPIN_SENSOR, OUTPUT);
  pinMode(CLOCKPIN_SENSOR, OUTPUT);
  delay(100);

  for(uint8_t i = 0; i<8; i++) {
    digitalWrite(LATCHPIN_SENSOR, LOW);
    shiftOut(DATAPIN_SENSOR, CLOCKPIN_SENSOR, MSBFIRST, PATTERNS[i]);
    digitalWrite(LATCHPIN_SENSOR,  HIGH);
    delay(1000);
    first_vals[i] = analogRead(0);
    first_vals[i+8] = analogRead(1);
    first_vals[i+16] = analogRead(2);
    first_vals[i+24] = analogRead(3);
    first_vals[i+32] = analogRead(4);
    first_vals[i+40] = analogRead(5);
    delay(100);
    AP_msg_L.data=first_vals;
    pub_temp.publish(&AP_msg_L);
    nh.spinOnce();
  }
}

void loop() {
  for(uint8_t i = 0; i<8; i++) {
    digitalWrite(LATCHPIN_SENSOR, LOW);
    shiftOut(DATAPIN_SENSOR, CLOCKPIN_SENSOR, MSBFIRST, PATTERNS[i]);
    digitalWrite(LATCHPIN_SENSOR,  HIGH);
    delay(20);
    vals[i] = analogRead(0);
    vals[i+8] = analogRead(1);
    vals[i+16] = analogRead(2);
    vals[i+24] = analogRead(3);
    vals[i+32] = analogRead(4);
    vals[i+40] = analogRead(5);
    dif_vals[i]=vals[i]-first_vals[i];
    dif_vals[i+8]=vals[i+8]-first_vals[i+8];
    dif_vals[i+16]=vals[i+16]-first_vals[i+16];
    dif_vals[i+24]=vals[i+24]-first_vals[i+24];
    dif_vals[i+32]=vals[i+32]-first_vals[i+32];
    dif_vals[i+40]=vals[i+40]-first_vals[i+40];
  }

  AP_msg_L.data = vals;
  pub_temp.publish(&AP_msg_L);
  AP_msg_L_Dif.data = dif_vals;
  pub_temp3.publish(&AP_msg_L_Dif);
  nh.spinOnce();
  delay(10);
}
