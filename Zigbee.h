#ifndef _c_Zigbee_H_
#define _c_Zigbee_H_
#pragma once

#include "Arduino.h"
#include "src/Math.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

class Zigbee
{
  public:    
    int data(int data_length,int data[]);
    
    char send(int sent_port,int rev_port,uint add,int Data_length,int Data[]);
    int  rev();
    
    int  setADD(int sent_port,uint new_add);
    int  setID(int sent_port,uint new_id);
    int  setROUTE(int sent_port,int route);
    int  setBAUDRATE(int sent_port,ulong new_baudrate);
        
    int  setLED(int sent_port,uint add,float Led_time);
    void setTTL(int sent_port,int rev_port,uint add,int operation);

    int  setRESET(int sent_port,uint add,uint id,uint rout,ulong baudrate);
    int  restart(int sent_port);
    
    int  readADD(int sent_port);
    int  readID(int sent_port);
    int  readROUT(int sent_port);
    int  readBAUDRATE(int sent_port);
    int  readRSSI(int sent_port,uint ID_1,uint ID_2);

  private:
    int port(int port);
    uint add(uint add);
    uint id(uint id);
    int route(int route);
    int baudrate(ulong baudrate);
    uint answer(int *data);
    int reply(int *data);
    
};


#endif

