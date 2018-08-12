#include <Mirf.h>                                        //发射端
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>                       
#include <stdlib.h>                                     //2.4g的库
#include <Wire.h>                                
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>                           //显示屏的库
//#include "Suanfa.h"                                    //算法的库.
#define   OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);                   //初始化显示屏
//Suanfa ad;                                            //创建一个对象
const int X_dir = 10;
const int Y_dir = 6;
double X_juli, Y_juli;
const byte X_interruptPin = 2;
const byte Y_interruptpin = 3;
volatile byte xstate = HIGH;
volatile byte ystate = HIGH;
double X,Y,p1,p0;
double data[3];
int YPR[3];
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
boolean flag = false;
void setup()
{
  Mrif1();    //2.4g通讯初始化
  display.begin(SSD1306_SWITCHCAPVCC);                    //显示屏初始化
  pinMode(X_dir, INPUT_PULLUP);                    
  pinMode(X_interruptPin, INPUT_PULLUP);
  pinMode(Y_dir, INPUT_PULLUP);
  pinMode(Y_interruptpin, INPUT_PULLUP);                  //中断引脚初始化             
  Serial.begin(115200);
  delay(2000);
  Serial.write(0XA5); 
  Serial.write(0X45);    
  Serial.write(0XEA);                                //初始化GY953,连续输出模式     
 
}
void loop()
{
  angle();
  Serial.println("156");
  attachInterrupt(digitalPinToInterrupt(X_interruptPin), xblink, FALLING );
  attachInterrupt(digitalPinToInterrupt(Y_interruptpin), yblink, FALLING );//编码器中断，计数
  X_juli = 1.0 * X / 256 * 119.73;
  Y_juli = 1.0 * Y / 256 * 119.73;                                        
  //ad.realXY(X_juli,Y_juli,p1);
  data[0] = X_juli;
  data[1] = Y_juli;
  data[2] = p1;
  Mirf.send((byte *)&data);
  while(Mirf.isSending())
  
  Serial.print(data[0]);                 //串口显示
  Serial.print("   ");                   //串口显示
  Serial.print(data[1]);                 //串口显示
  Serial.print("   ");                   //串口显示
  Serial.println(data[2]);  //串口显示
  
  delay(1);
  view( X_juli,Y_juli,p1);
}
void xblink()
{
  xstate = digitalRead(X_dir);
  if (xstate == HIGH)
    X--;
  else
    X++;
}
void yblink()
{
  ystate = digitalRead(Y_dir);
  if (ystate == HIGH)
    Y++;
  else
    Y--;
}
void serialEvent() 
{
  while (Serial.available()) 
  {   
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==11)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;} 
    }      
}
void Mrif1()
{
  Mirf.spi = &MirfHardwareSpi;  
  Mirf.init();
  Mirf.setRADDR((byte *)"12345");          
  Mirf.setTADDR((byte *)"67890");          
  Mirf.payload = sizeof(data);
  Mirf.channel=3;                          
  Mirf.config();
  }
void angle()
{ 
  if (sign)
  {
    sign = 0;
    if (Re_buf[0] == 0x5A && Re_buf[1] == 0x5A ) //检查帧头，帧尾
    {
      YPR[0]=(Re_buf[8]<<8|Re_buf[9])/100;   //合成数据，去掉小数点后2位
      YPR[1]=(Re_buf[6]<<8|Re_buf[7])/100;
      YPR[2]=(Re_buf[4]<<8|Re_buf[5])/100;
    }
  
   /*if(!flag)
    {
      p0 = YPR[0];
     }
     flag = true ;  */
     p1 = YPR[0];
   //ad.tozero(p1,p0);
  }
}
void view(double x,double y,double p)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("x = ");
  display.println(x);
  display.print("y = ");
  display.println(y);
  display.print("P = ");
  display.println(p);
  display.display();
  delay(1);
  }
