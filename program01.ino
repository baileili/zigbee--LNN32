#include "zigbee.h"


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  zigbee_rev();
//  int a[14]={253,1,254,253,4,254,252,254,253,9,10,11,12,255};
//  Serial.println(translation(a, sizeof(a)/sizeof(int)));
//  while(1);
}
