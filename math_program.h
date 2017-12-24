  /******************************************************************************************
                                  数据数学关系处理函数
 /*****************************************************************************************/
 
#include "Arduino.h"

#define erro 0
#define end 1

/******************************************************************************************
函数名称：array_memset(int *array_name,int array_set,int array_length)
函数作用：将int数形数组全部清零
输入参数：*array_name ——————————指针（指向数组的第一个元素地址，每次使用需要将
                                               指针地址更新）
          array_set   ——————————设置值（清零时为NULL或者0）
          array_length——————————数组长度
输出参数：无
测试例程：int a[]={2,3,4,5,6};
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          Serial.println();
          array_memset(a,NULL,sizeof(a)/sizeof(int));
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          while(1);
返回参数：23456\n00000
 /*****************************************************************************************/
 int array_memset(int *array_name,int array_set,int array_length)
{
  for(int i=0;i<array_length;i++)
  {
    *array_name=array_set;
    *array_name++; 
  }
} 

/******************************************************************************************
函数名称：ascall_change(int  data)
函数作用：按ASCALL码表将对应char数型转换成int数型
输入参数： data       ——————————输入数据
输出参数：对应的int数型格式数据
测试例程：while(Serial.available() > 0)
         {   
            c=char(Serial.read());
            Serial.print("  HEX=");Serial.print(c); 
            int b;
            b=ascall_change(c);
            Serial.print("    DEC=");Serial.println(b);     
          }
返回参数：输入1返回(  HEX=1    DEC=1)
          输入a返回(  HEX=a    DEC=10)
/*****************************************************************************************/
int ascall_change(int  data)
{
  int data_change;
  if(data>47&&data<58)data_change=(data-48);
  else if(data>64&&data<71)data_change=(data-55);
  else if(data>96&&data<103)data_change=(data-87);
  else data_change=15;
  return data_change;
}

/******************************************************************************************
函数名称：data_rate(char data_char01,char data_char02)
函数作用：将两个16进制数组处理成10进制内容(例如：16进制为FE，处理成二进制为254)
输入参数： data_char02——————————输入16进制第一个内容
           data_char01——————————输入16进制第二个内容
输出参数：对应的int数型格式数据
测试例程：//全局变量
          int data_cout=0;char data_char01,data_char02;

          //**********主循环内容**********
          //串口输入触发函数
           while(Serial.available()>0)
            {
              //奇数处理方式
              if(data_cout%2==0)
                {data[data_char01]=Serial.read();}
              //偶数处理方式
              else 
                {data[data_char02]=Serial.read();data_rate(data_char01,data_char02);}
              data_cout++;
            }                          
返回参数：输入10返回(  HEX=1 0    DEC=17)
          输入fe返回(  HEX=f e    DEC=254)
          输入FF返回(  HEX=F F    DEC=255)
/*****************************************************************************************/
int data_rate(char data_char01,char data_char02)
{
  int data=ascall_change(data_char01)*16+ascall_change(data_char02);
  //Serial.print("   HEX=");Serial.print(data_char01);Serial.print(" ");Serial.print(data_char02);
  //Serial.print("   DEC=");Serial.println(data);
  return data;  
}

/******************************************************************************************
函数名称：int translation(int *data, int data_length)
函数作用：返转义，同时返回有效数据数组长度(包括数据头与数据尾)
输入参数： 
输出参数：有效数据长度(int数型)
测试例程：int a[14]={253,1,254,253,4,254,252,254,253,9,10,11,12,255};
          Serial.println(translation(a, sizeof(a)/sizeof(int)));
          while(1);
返回参数：11
/*****************************************************************************************/
int translation(int *data, int data_length)
{
  int flag=0;int data_stored;
  for(int i=0;i<data_length;i++,*data++)
  {
    if(i==0&&*data!=253)return erro;
    else if(i==(data_length-1)&&*data!=255)return erro;
    else if(*data==254)
      {*data++;if(*data==252||*data==253){data_stored=*data;*data=0;*data--;*data=(data_stored+2);}}
  }
  for(int i=0;i<data_length;i++)*data--;
  //Serial.println("---------------------------------------------");
  for(int i=0;i<data_length;i++,*data++)
  {
    if(*data==0){*data--;if(*data==255||*data==254){flag++;}*data++;}
    line_a:for(int j=0;j<flag;j++)*data++;
    if(*data==0){*data--;if(*data==255||*data==254){flag++;goto line_a;}}data_stored=*data;
    for(int j=0;j<flag;j++)*data--;
    *data=data_stored;
  }
  for(int i=0;i<data_length;i++)*data--;
  //for(int i=0;i<(data_length-flag);i++,*data++)
    //{Serial.print("data");Serial.print(i);Serial.print("=");Serial.println(*data);}
  //for(int i=0;i<(data_length);i++)*data--;
  return (data_length-flag);
}
