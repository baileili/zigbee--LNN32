#ifndef _c_Math_H_
#define _c_Math_H_

#include "Arduino.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long



class Math
{
  public:
   friend class Zigbee;
   int cout=0;
   char char_data0=0;
   char char_data1=0;
   int data_length;
   int translation(int *data, int data_length,bool Ser_printf=false); 
   unsigned int reply(int *data);

 protected:
   int datagroup(char data_0,char data_1);
   int READOUTtoBAUDRATE(unsigned long baudrate);
   int memset(int *array_name,int array_length,int array_set=NULL);
   
 private: 
   int flag=0;
   int data_stored;
   int ASCALLtoHEX(int  data);
      
};

#endif

