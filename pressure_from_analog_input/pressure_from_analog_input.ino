#include <ros.h>
#include <std_msgs/Int16MultiArray.h>

#define ANALOG_INPUT_SIZE 6

ros::NodeHandle nh;
std_msgs::Int16MultiArray p;
ros::Publisher pub("airpressure", &p);
int16_t a[6] = {};


void setup()
{
  // nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(pub);
  p.data_length = ANALOG_INPUT_SIZE;
  p.data = (int16_t *)malloc(sizeof(int16_t)*ANALOG_INPUT_SIZE);
  delay(1000); // warm-up time for sensor at least: 20 msec
}

void loop()
{
  for (int i=0; i<ANALOG_INPUT_SIZE; i++) {
    a[i] = analogRead(i);
  }
  p.data = a;
  pub.publish( &p );
  nh.spinOnce();
}
