/******************************************************************************************
                                  数据Zigbee数据类型函数
 /*****************************************************************************************/

#include "zigbee.h"
#include "Arduino.h"


Zigbee zigbee;
Math zigbee_math;

bool rev_flag=false;
int steps_flag1=0,steps_flag2=0;
int data_cout=0;
int rev_data[100];
uint long time_millis;

/******************************************************************************************
函数名称：Zigbee.port(int port)
函数作用：按DL-LN3X格式处理端口号数据，并发送出去
输入参数：port  ——————————待处理端口号
输出参数：无
测试例程：zigbee_port(241);while(1);
返回参数：F1//端口号为241（16进制为F1）
 /*****************************************************************************************/
int Zigbee::port(int port)
{
  if(port==255)
    Serial.print("FEFD");
  else if(port==254)Serial.print("FEFC");
  else if(port<=15){Serial.print("0");Serial.print(port,HEX);}
  else  Serial.print(port,HEX);  
}

/******************************************************************************************
函数名称：Zigbee.add(uint add)
函数作用：按DL-LN3X格式处理远程地址数据，并发送出去
输入参数：add  ——————————待远程地址号
输出参数：无
测试例程：zigbee.ID(4097);while(1);
返回参数：0101//端口号为4097（16进制为1001）
 /*****************************************************************************************/
uint Zigbee::add(uint add)
{
  //端口号(add)在 0000(0) 和 000F(15) 之间
  if(add<=15)
    {Serial.print("0");Serial.print(add,HEX);Serial.print("00");}

  //端口号(add)在 0010(16) 和 00FF(255) 之间
  else if(add>15&&add<=255)
    {
      //输出低8位数据
      if(add==255)Serial.print("FEFD");
      else if (add==254)Serial.print("FEFC");
      else Serial.print(add,HEX);
      //输出高8位数据
      Serial.print("00");
    }

  //端口号(add)在 0100(256) 和 0FFF(4095) 之间
  else if(add>255&&add<=4095)
    {
      //输出低8位数据
      if(add%256==255)Serial.print("FEFD");
      else if (add%256==254)Serial.print("FEFC");
      else {Serial.print(((add%256)/16),HEX);Serial.print(((add%256)%16),HEX);}
      //输出高8位数据
      Serial.print("0");Serial.print((add/256),HEX);
    }

  //端口号(add)在 1000(4095) 和 FFFF(65536) 之间
  else if(add>4095&&add<=65536)
    {    
      //输出低8位数据  
      if((add%4096)%256==255)Serial.print("FEFD");
      else if((add%4096)%256==254)Serial.print("FEFC");
      else {Serial.print(((add%4096)%256)/16,HEX);Serial.print(((add%4096)%256)%16,HEX);}
      //输出高8位数据
      if(add/256==255)Serial.print("FEFD");
      else if(add/256==254)Serial.print("FEFC");
      else {Serial.print((add/4096),HEX);Serial.print((add%4096)/256,HEX);}
    }  
}

/******************************************************************************************
函数名称：Zigbee.route(int route)
函数作用：按DL-LN3X格式处理信道，并发送出去
输入参数：route      ——————————信道
输出参数：无
测试例程：zigbee_route(19);while(1);
返回参数：13//以16进制输出19(十六进制为13)
 /*****************************************************************************************/
int Zigbee::route(int route)
{
  if(route<=15)Serial.print("0");
  Serial.print(route,HEX);
}

/******************************************************************************************
函数名称：Zigbee.id(uint id)
函数作用：按DL-LN3X格式处理网络id(ID)，并发送出去
输入参数：route      ——————————信道
输出参数：无
测试例程：Zigbee.id(19);while(1);
返回参数：13//以16进制输出19(十六进制为13)
 /*****************************************************************************************/
uint Zigbee::id(uint id)
{
  //端口号(ID)在 0000(0) 和 000F(15) 之间
  if(id<=15)
    {Serial.print("0");Serial.print(id,HEX);Serial.print("00");}

  //端口号(add)在 0010(16) 和 00FF(255) 之间
  else if(id>15&&id<=255)
    {
      //输出低8位数据
      if(id==255)Serial.print("FEFD");
      else if (id==254)Serial.print("FEFC");
      else Serial.print(id,HEX);
      //输出高8位数据
      Serial.print("00");
    }

  //端口号(ID)在 0100(256) 和 0FFF(4095) 之间
  else if(id>255&&id<=4095)
    {
      //输出低8位数据
      if(id%256==255)Serial.print("FEFD");
      else if (id%256==254)Serial.print("FEFC");
      else {Serial.print(((id%256)/16),HEX);Serial.print(((id%256)%16),HEX);}
      //输出高8位数据
      Serial.print("0");Serial.print((id/256),HEX);
    }

  //端口号(ID)在 1000(4095) 和 FFFF(65536) 之间
  else if(id>4095&&id<=65536)
    {    
      //输出低8位数据  
      if((id%4096)%256==255)Serial.print("FEFD");
      else if((id%4096)%256==254)Serial.print("FEFC");
      else {Serial.print(((id%4096)%256)/16,HEX);Serial.print(((id%4096)%256)%16,HEX);}
      //输出高8位数据
      if(id/256==255)Serial.print("FEFD");
      else if(id/256==254)Serial.print("FEFC");
      else {Serial.print((id/4096),HEX);Serial.print((id%4096)/256,HEX);}
    }  
}

/******************************************************************************************
函数名称：Zigbee.baudrate(ulong baudrate)
函数作用：按DL-LN3X格式处理波特率，并发送出去
输入参数：baudrate  ——————————波特率
输出参数：无
测试例程：zigbee_baudrate(9600);while(1);
返回参数：13//以16进制输出19(十六进制为13)
 /*****************************************************************************************/
int Zigbee::baudrate(ulong baudrate)
{
  if(baudrate==2400)Serial.print("00");
  else if(baudrate==4800) Serial.print("01");
  else if(baudrate==9600) Serial.print("02");
  else if(baudrate==14400)Serial.print("03");
  else if(baudrate==19200)Serial.print("04");
  else if(baudrate==28800)Serial.print("05");
  else if(baudrate==38400)Serial.print("06");
  else if(baudrate==57600)Serial.print("07");
  else if(baudrate==115200)Serial.print("08");
  else if(baudrate==230400)Serial.print("09");
  else if(baudrate==125000)Serial.print("0A");
  else if(baudrate==250000)Serial.print("0B");
  else if(baudrate==500000)Serial.print("0C");
  else Serial.print("02");
}

/******************************************************************************************
函数名称：Zigbee.data(int data_length,int data[])
函数作用：按DL-LN3X格式处理数据，并发送出去
输入参数：data_length——————————数据长度
          data[]     ——————————数据内容存储数组
输出参数：无
测试例程：int a[]={10,31,45,1};zigbee_data(sizeof(a)/sizeof(int),a);while(1);
返回参数：0A1F2D01//以16进制输出数组内容
 /*****************************************************************************************/
int Zigbee::data(int data_length,int data[])
{
  for(int i=0;i<data_length;i++)  
    {     
      //Serial.print("message[");Serial.print(i);Serial.print("]=");
      //传输数据格式处理
      if(data[i]==0)Serial.print("00");else if(data[i]<17&&data[i]>0)Serial.print("0");
      if(data[i]==255)Serial.print("FEFD");else if(data[i]==254)Serial.print("FEFC");
      else Serial.print(data[i],HEX);
      //释放data数组内存
      data[i]=NULL;   
    }
}

/******************************************************************************************
函数名称：Zigbee.send(int sent_port,int rev_port,uint add,int data_length,int data[])
函数作用：按DL-LN3X格式处理数据，并发送出去
输入参数：sent_port ——————————源端口号
rev_port ——————————目的端口号
add ——————————远程地址
data_length——————————数据长度
data[] ——————————数据内容
输出参数：无
测试例程：int a[]={10,31,45,1};zigbee_send(145,144,768,sizeof(a)/sizeof(int),a);while(1);
返回参数：FE08919003000A1F2D01FF//数组内容（16进制格式）：0A1F2D01
/*****************************************************************************************/
char Zigbee::send(int sent_port,int rev_port,uint add,int Data_length,int Data[])
{
  Serial.print("FE");
  //数据长度处理
  if(Data_length<11)Serial.print("0");Serial.print(Data_length+4);
  //源端口号格式处理
  zigbee.port(sent_port);
  //目的端口号格式处理
  zigbee.port(rev_port);
  //远程地址格式处理
  zigbee.add(add);
  //数据处理
  zigbee.data(sizeof(Data)/sizeof(int),Data);
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.setADD(int sent_port,uint ADD)
函数作用：设置模块地址(ADD)
输入参数：sent_port  ——————————源端口号
          add       ——————————新的地址(ID)
输出参数：无
测试例程：zigbee_set_ID(145,65029);while(1);
返回参数：FE07912100001105FEFF//数据是0X11,新地址(ID)是FE05
 /*****************************************************************************************/
int Zigbee::setADD(int sent_port,uint new_add)
{
    Serial.print("FE07"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("11");
  //新地址
  zigbee.add(new_add);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.setID(int sent_port,uint new_id)
函数作用：设置模块地址(ADD)
输入参数：sent_port  ——————————源端口号
          add       ——————————新的地址(ID)
输出参数：无
测试例程：zigbee_set_ID(145,65029);while(1);
返回参数：FE07912100001105FEFF//数据是0X11,新地址(ID)是FE05
 /*****************************************************************************************/
int Zigbee::setID(int sent_port,uint new_id)
{
    Serial.print("FE07"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X12)
  Serial.print("12");
  //新地址
  zigbee.id(new_id);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.setROUTE(int sent_port,int route)
函数作用：设置模块信道
输入参数：sent_port  ——————————源端口号
          route      ——————————新信道
输出参数：无
测试例程：zigbee_set_route(145,21);while(1);
返回参数：FE07912100001313FF//数据是0X13,信道是19(十六进制为13)
 /*****************************************************************************************/
int Zigbee::setROUTE(int sent_port,int route)
{
    //Serial.print("FE07"); 
  Serial.print("FE07");
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("13");
  //新地址
  zigbee.route(route);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.setBAUDRATE(int sent_port,int new_baudrate)
函数作用：读取模块波特率
输入参数：sent_port   ——————————源端口号
          new_baudrate——————————新波特率
输出参数：无
测试例程：zigbee_set_baudrate(145,3600);while(1);
返回参数：FE06912100001402FF//设置波特率是9600(0X02)
 /*****************************************************************************************/
int Zigbee::setBAUDRATE(int sent_port,ulong new_baudrate)
{

  Serial.print("FE06"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("14");
  //设置波特率
  zigbee.baudrate(new_baudrate);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_restart(int sent_port)
函数作用：重启模块
输入参数：无
输出参数：无
测试例程：zigbee_restart(145);while(1);
返回参数：FE06912100001402FF//设置波特率是9600(0X02)
 /*****************************************************************************************/
int Zigbee::restart(int sent_port)
{
  
  Serial.print("FE05"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X10)
  Serial.print("10");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.setLED(int sent_port,uint add,float Led_time)
函数作用：点亮LED
输入参数：sent_port ——————————源端口号
ID ——————————远程地址(ID)
Led_time ——————————LED点亮时间(单位为 S)
输出参数：无
测试例程：Zigbee.setLED(145,0,0.5);while(1);
返回参数：FE07912100002005FF//点亮LED0.5s
/*****************************************************************************************/
int  Zigbee::setLED(int sent_port,uint add,float Led_time)
{
  Serial.print("FE06");
  //源端口号
  zigbee.port(sent_port);
  //目的端口号(0X20)
  Serial.print("20");
  //原远程地址
  zigbee.add(add);
  //LED点亮时间 Led_time s
  int led_time=Led_time*10;
  if(led_time<=15)Serial.print("0");
  Serial.print(led_time,HEX);
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee::readADD(int sent_port)
函数作用：读取模块地址
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_ID(145);while(1);
返回参数：FE059121000001FF//数据是(0X0000)命令是(0X01)
 /*****************************************************************************************/
int Zigbee::readADD(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X01)
  Serial.print("01");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.readnetID(int sent_port)
函数作用：读取模块网络ID地址(ID)
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_netID(145);while(1);
返回参数：FE059121000002FF//数据是0X02
 /*****************************************************************************************/
int Zigbee::readID(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X02)
  Serial.print("02");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.readROUT(int sent_port)
函数作用：读取模块信道
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_route(145);while(1);
返回参数：FE059121000003FF//数据是0X03
 /*****************************************************************************************/
int Zigbee::readROUT(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X03)
  Serial.print("03");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee.readBAUDRATE(int sent_port)
函数作用：读取模块波特率
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_baudrate(145);while(1);
返回参数：FE059121000004FF//数据是0X04
 /*****************************************************************************************/
int Zigbee::readBAUDRATE(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee.port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("04");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：Zigbee.readRSSI(int sent_port,unit ID_1,unit ID_2)
函数作用：测试连接质量
输入参数：sent_port  ——————————源端口号
          ADD_1      ——————————测试对象1模块地址
          ADD_1     ——————————测试对象2模块地址
输出参数：需调用zigbee_rev()函数处理返回的连接质量参数
测试例程：未测试
返回参数：
 /*****************************************************************************************/
int Zigbee::readRSSI(int sent_port,uint ADD_1,uint ADD_2)
{
  //串口输出采集数据包
  Serial.print("FE06");
  Serial.print(zigbee.port(sent_port));
  Serial.print("23");
  Serial.print(zigbee.add(ADD_1));  Serial.print(zigbee.add(ADD_2));
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_rev()
函数作用：接收串口数据，并按照zigbee格式内容进行处理
输入参数：无
输出参数：返回接收数据长度
测试例程：zigbee_rev();
返回参数：串口输入(0102030405060708090A0B0C0D0E0F)
          未取消注释(   data[0]=1 \n data[1]=2 \n data[2]=3 \n data[3]=4 \n
                        data[4]=5 \n data[5]=6 \n data[6]=7 \n data[7]=8 \n
                        data[8]=9 \n data[9]=10\n data[10]=11 \n data[11]=12 \n 
                        data[12]=13 \n data[13]=14 \n data[14]=15)
 *****************************************************************************************/
int Zigbee::rev()
{
  //串口输入开始接收信息
  while(Serial.available()>0)
  {
   //数组开始接收串口输入信息
   rev_data[data_cout]=Serial.read();
   //串口数据标志符置高
   rev_flag=true;
   //装载时间校准数据，防止误操作
   time_millis=millis();
    //偶数位数据为一直字符数据，开始处理
    if(data_cout%2!=0)
      {
        //装载时间校准数据，开始记录处理时间
        time_millis=millis();
        //处理数据，并转成int数型储存在数组中
        rev_data[(data_cout/2)]=zigbee_math.datagroup(rev_data[(data_cout-1)],rev_data[data_cout]); 
      }
    //数据记录操作
    data_cout++;
    return 0;
  }
  //接收超时，接收完毕
  if((time_millis+20)<=millis()&&rev_flag==true)
  {
    //串口接收标识符置低
    rev_flag=false;
    //反转义接收到的数据(删除转义的内容)
    zigbee_math.translation(rev_data,(data_cout/2)); 
    //检测是否为特殊定义的回复包内容
    if(zigbee_math.reply(rev_data)==1){steps_flag1=0;steps_flag2++;}
    
    //串口输出数据
    for(int i=0;i<data_cout;i=i+2)
    {Serial.print("   data[");Serial.print(i/2);Serial.print("]=");Serial.println(rev_data[i/2]);}    
    //返回数据长度
    Serial.print("data length=");Serial.println(data_cout/2);
    int data_cout_stored=data_cout/2;data_cout=0;
    return data_cout_stored;
  }
}

/******************************************************************************************
函数名称：zigbee_ttl(int sent_port,int rev_port,unit ID,int operation)
函数作用：可控TTL输出
输入参数：sent_port  ——————————源端口号
          sent_port  ——————————接收端口号
          ID         ——————————模块地址
          operation  ——————————操作方式(高低电平与读取输出的电平值)
输入参数：无
测试例程：Pin4高电平 zigbee_ttl(6,TTL_Pin4,0,TTL_HIGH);
          Pin5读电平 zigbee_ttl(6,TTL_Pin5,0,TTL_READ);
          未测试
返回参数：无
 /*****************************************************************************************/
void Zigbee::setTTL(int sent_port,int rev_port,uint add,int operation)
{
  Serial.print("EF05");
  Serial.print(zigbee.port(sent_port));
  Serial.print(zigbee.port(rev_port)); 
  Serial.print(zigbee.add(add)); 
  Serial.print(operation);
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_ttl(int sent_port,int rev_port,unit ID,int operation)
函数作用：可控TTL输出
输入参数：sent_port  ——————————源端口号
          sent_port  ——————————接收端口号
          ID         ——————————模块地址
          operation  ——————————操作方式(高低电平与读取输出的电平值)
输入参数：无
测试例程：Pin4高电平 zigbee_ttl(6,TTL_Pin4,0,TTL_HIGH);
          Pin5读电平 zigbee_ttl(6,TTL_Pin5,0,TTL_READ);
          未测试
返回参数：无
 /*****************************************************************************************/
int Zigbee::setRESET(int sent_port,uint add,uint id,uint route,ulong baudrate)
{
  if(steps_flag1==0)
    {
      if(steps_flag2==0)
        {Serial.begin(115200);Serial.print("step 1:");zigbee.setADD(sent_port,add);steps_flag1=1;}
      else if(steps_flag2==1)
        {Serial.print("step 2:");zigbee.setID(sent_port,id);steps_flag1=1;}
      else if(steps_flag2==2)
        {Serial.print("step 3:");zigbee.setROUTE(sent_port,route);steps_flag1=1;}
      else if(steps_flag2==3)
        {Serial.print("step 4:");zigbee.setBAUDRATE(sent_port,baudrate);steps_flag1=1;}
    }
  else if(steps_flag1==1)
    {zigbee.rev();}
  return 1;
}
