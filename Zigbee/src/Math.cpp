/******************************************************************************************
                                  数据Zigbee数据类型函数
 /*****************************************************************************************/

#include "Zigbee.h"
#include "Arduino.h"

#define erro 0
#define end 1


Zigbee zigbee;
Math math;
Math zigbee_math;

#define uint unsigned int
#define uchar unsigned char

bool rev_flag=false;
int steps_flag1=0,steps_flag2=0;
int data_cout=0;
int rev_data[100];
uint long time_millis;

/******************************************************************************************
函数名称：Math.memset(int *array_name,int array_set,int array_length)
函数作用：将int数形数组全部清零
输入参数：*array_name ——————————指针（指向数组的第一个元素地址，每次使用需要将
                                               指针地址更新）
          array_set   ——————————设置值（清零时为NULL或者0）
          array_length——————————数组长度
输出参数：无
测试例程：int a[]={2,3,4,5,6};
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          Serial.println();
          Math.memset(a,sizeof(a)/sizeof(int),NULL);
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          while(1);
返回参数：23456\n00000
 /*****************************************************************************************/
 int Math::memset(int *array_name,int array_length,int array_set=NULL)
{
  for(int i=0;i<array_length;i++)
  {
    *array_name=array_set;
    *array_name++;
  }
}

/******************************************************************************************
函数名称：Math.ASCALLtoHEX(int  data)
函数作用：按ASCALL码表将对应char数型转换成 数型
输入参数： data       ——————————输入数据
输出参数：对应的int数型格式数据
测试例程：while(Serial.available() > 0)
         {   
            char c=char(Serial.read());
            Serial.print("  HEX=");Serial.print(c); 
            int b;
            b=Math.ASCALLtoHEX(c);
            Serial.print("    DEC=");Serial.println(b);     
          }
返回参数：输入1返回(  HEX=1    DEC=1)
          输入a返回(  HEX=a    DEC=10)
/*****************************************************************************************/
int Math::ASCALLtoHEX(int data)
{
  int data_change;
  if(data>47&&data<58)data_change=(data-48);
  else if(data>64&&data<71)data_change=(data-55);
  else if(data>96&&data<103)data_change=(data-87);
  else data_change=15;
  return data_change;
}

/******************************************************************************************
函数名称：Math.datagroup(char data_0,char data_1)
函数作用：将两个16进制数组处理成10进制内容(例如:十六进制为FE，处理成二进制为254)
输入参数： data0——————————输入16进制第一个内容
           data1——————————输入16进制第二个内容
输出参数：对应的int数型格式数据(串口输出需要取消注释)
测试例程：while(Serial.available()>0)
            {
              if(Math.cout%2==0)
                {Math.char_data0=Serial.read();                  
                }
              else
                {
                  Math.char_data1=Serial.read();
                  Serial.print("   HEX=");Serial.print(data_0);
                  Serial.print(" ");Serial.print(data_1);
                  Serial.print("   DEC=");
                  Serial.print(Math.datagroup(Math.char_data0));
                }
              Math.cout++;
            }                          
返回参数：输入10返回17(  HEX=1 0    DEC=17)
          输入fe返回254(  HEX=f e    DEC=254)
          输入FF返回255(  HEX=F F    DEC=255)
/*****************************************************************************************/
int Math::datagroup(char data_0,char data_1)
{
  int data=math.ASCALLtoHEX(data_0)*16+math.ASCALLtoHEX(data_1);
  return data;  
}

/******************************************************************************************
函数名称：Math.READOUTtoBAUDRATE(ulong baudrate)
函数作用：接收到DL-LN的波特率读书转换成波特率，用于调试
输入参数：baudrate ——————————波特率
输出参数：无
测试例程：zigbee_rebaudrate(2);while(1);
返回参数：9600
/*****************************************************************************************/
int Math::READOUTtoBAUDRATE(unsigned long baudrate)
{
  if(baudrate==0)
    {Serial.print("2400");return 2400;}
  else if(baudrate==1)
    {Serial.print("4800");return 2400;}
  else if(baudrate==2)
    {Serial.print("9600");return 9600;}
  else if(baudrate==3)
    {Serial.print("14400");return 14400;}
  else if(baudrate==4)
    {Serial.print("19200");return 19200;}
  else if(baudrate==5)
    {Serial.print("28800");return 28800;}
  else if(baudrate==6)
    {Serial.print("38400");return 38400;}
  else if(baudrate==7)
    {Serial.print("57600");return 57600;}
  else if(baudrate==8)
    {Serial.print("115200");return 115200;}
  else if(baudrate==9)
    {Serial.print("230400");return 230400;}
  else if(baudrate==10)
    {Serial.print("125000");return 125000;}
  else if(baudrate==11)
    {Serial.print("250000");return 250000;}
  else if(baudrate==12)
    {Serial.print("500000");return 500000;}
  else 
    Serial.print("9600");
}

/******************************************************************************************
函数名称：int translation(int *data, int data_length)
函数作用：返转义，同时返回有效数据数组长度(包括数据头与数据尾)
输入参数： 
输出参数：有效数据长度(int数型)
测试例程：int a[14]={253,1,254,253,4,254,252,254,253,9,10,11,12,255};
          int data_length=Math.translation(a, sizeof(a)/sizeof(int));
          while(1); 
返回参数：11(原转义数据：253, 1,254,253, 4,254,252,254,253,9 ,10 ,11,12,255)
            (反转义数据：253, 1,255，  , 4,254,   ,255,   ,9 ,10 ,11,12,255)
            (HEX内容   ：FD ,01,FF ,   ,04,FE ,   ,FF ,   , 09,0A,0B,0C,FF)
            FD01FF04FEFF090A0B0CFF
/*****************************************************************************************/
int Math::translation(int *data, int data_length,bool Ser_printf)
{
  //声明局部变量math.flag(记录转义次数)
  math.flag=0;
  //声明局部变量math.data_stored(数据暂存)
  math.data_stored;
  if(Ser_printf==true)
    {Serial.println();Serial.print("the Original data is:");}
  //将转义数据反转义，并将多余数据标记为0
  for(int i=0;i<data_length;i++,*data++)
  {
    //串口打印数据
    if(Ser_printf==true){Serial.print(*data);Serial.print("  ");}
    //检测数据头标记符是否存在，判断接收数据是否正确
    if(i==0&&*data!=253)
      return erro;
    //检测数据尾标记符是否存在，判断接收数据是否正确
    else if(i==(data_length-1)&&*data!=255) 
      return erro;
    //将转义内容反转义(FEFD>>FF ; FEFC>>FE)
    else if(*data==254)
      {
        //检测下一个数据内容，判断是否为转义内容
        *data++;
        if(*data==252||*data==253)
          //反转义(FEFD>>FF00; FEFC>>FE00)
          {math.data_stored=*data;*data=0;*data--;*data=(math.data_stored+2);}
        else *data--;
      }
  }
  //将指针地址返回数组的首元素地址
  for(int i=0;i<data_length;i++)*data--;
  //Serial.println("---------------------------------------------");
  if(Ser_printf==true)
    {Serial.println();Serial.print("the   new  data   is:");}
  //删除转义多余数据(即0)
  for(int i=0;i<data_length;i++,*data++)
  {
    //检测是否为需删除数据
    if(*data==0)
      {*data--;if(*data==255||*data==254){math.flag++;}*data++;}
    //若数据为删除的数据，进行标记(math.flag),嗅探位移数据操作次数
    line_a:for(int j=0;j<math.flag;j++)*data++;
    //判断位移数据是否也为删除数据,并进行处理
    if(*data==0)
      {
        *data--;
        if(*data==255||*data==254)
          {math.flag++;goto line_a;}
      }
    //将位移数据储存在局部变量(math.data_stored)中
    math.data_stored=*data;
    //位移至需要位移的数据位置
    for(int j=0;j<math.flag;j++)*data--;
    //替换数据，实现数组位移
    *data=math.data_stored;
    if(Ser_printf==true&&i<(data_length-math.flag))
      {Serial.print(*data);Serial.print("  ");}
  }
  //指针位置复位至数组首数据位置
  for(int i=0;i<data_length;i++)*data--; 
   
  //返回真实数组数据长度
  return (data_length-math.flag);
}

/******************************************************************************************
函数名称：Math::reply(int *data)
函数作用：处理zigbee返回的特殊指令
          (响应包、地址包、网络地址包、波特率包)
输入参数：无
输出参数：返回接收数据长度
测试例程：int a[8]={254,5,33,144,0,0,0,255};
          zigbee_answer(a);
          while(1);
返回参数：无(串口显示 Operation completion )
 /*****************************************************************************************/
uint Math::reply(int *data)
{
  if(*data==254)  
  {    
    for(int i=0;i<2;i++)*data++;
    //确定是否为0X21端口数据
    if(*data==33)
    {
      for(int i=0;i<4;i++)*data++;
      //确定命令类型
      switch(*data)
        {
          //设置参数成功(0X00)，发送信息形如(FE052190000000FE)
          case 0:{
                    for(int i=0;i<6;i++)*data--;
                    Serial.println("Operation completion");
                    return end;
                  }
          //端口不可用(0XF0),发送信息形如(FE052190000F0FE)
          case 240:{
                    for(int i=0;i<6;i++)*data--;
                    Serial.println("Port unavailability");
                    return erro;
                  }
          //命令错误(0XF8),发送信息形如(FE052190000F0FE)
          case 248:{
                    for(int i=0;i<6;i++)*data--;
                    Serial.println("Command fault");
                    return erro;
                  }
          //包长度与命令要求不相符(0XF9),发送信息形如(FE052190000F8FE)
          case 249:{
                    for(int i=0;i<6;i++)*data--;
                    Serial.println("Length error");
                    return erro;
                  }
          //包值不可用,发送信息形如(FE052190000FAFE)
          case 250:{
                    for(int i=0;i<6;i++)*data--;
                    Serial.println("Numeric unavailability");
                    return erro;
                  }
          //读取地址(add)，返回地址(收到的信息形如 FE0721900000210F00FF )
          case 33:{
                    *data++;uint add_stored=*data;
                    *data++;add_stored=((*data*256)+add_stored);
                    for(int i=0;i<8;i++)*data--;
                    //串口显示地址
                    Serial.print("read add=");Serial.println(add_stored);
                    //返回地址
                    return add_stored;
                  }
          //读取网络地址(ID)，返回网络地址(收到的信息形如 FE0721900000228819F )
          case 34:{
                    *data++;uint ID_stored=*data;
                    *data++;ID_stored=((*data*256)+ID_stored);
                    for(int i=0;i<8;i++)*data--;
                    //串口显示网络地址
                    Serial.print("read ID=");Serial.println(ID_stored);
                    //返回网络地址
                    return ID_stored;
                  }
          //读取信道，返回信道(收到的信息形如 FE0721900000228819F )
          case 35:{
                    *data++;int rout_stored=*data;
                    for(int i=0;i<7;i++)*data--;
                    //串口显示信道
                    Serial.print("read rout=");Serial.println(rout_stored);
                    //返回信道
                    return rout_stored;
                  }
          //读取波特率，返回波特率(收到的信息形如 FE0721900000228819F )
          case 36:{
                    *data++;int baudrate_stored=*data;
                    for(int i=0;i<8;i++)*data--;
                    //串口显示波特率
                    Serial.print("read baudrate=");Serial.println(math.READOUTtoBAUDRATE(baudrate_stored));
                    //返回波特率对应代码
                    return baudrate_stored;
                  }
        }
    }

    //确认是否为0X23端口数据
    else if(*data==35)
    {
      Serial.print("RSSI test");
      //将指针返回至数据头  
      for(int i=0;i<2;i++)*data--;
    }
    else for(int i=0;i<2;i++)*data--;
  }
}
