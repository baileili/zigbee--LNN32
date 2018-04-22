#include "Zigbee.h"

Zigbee Zigbee;

void setup()
{  
	Serial.begin(9600);
  Zigbee.setADD(21,21);
}

void loop()
{

}
