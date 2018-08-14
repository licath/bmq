#include <Mirf.h>                                        //发射端
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <stdlib.h>                                     //2.4g的库
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>                           //显示屏的库56
#include "Suanfa.h"                                     //算法的库.
#include <JY901.h>
#define   OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);                   //初始化显示屏
Suanfa ad;                                            //创建一个对象
const int X_dir = 5;
const int Y_dir = 6;
double X_juli, Y_juli;
const byte X_interruptPin = 2;
const byte Y_interruptpin = 3;
volatile byte xstate = HIGH;
volatile byte ystate = HIGH;
double X, Y, p1, p0;
double data[3];
double Ang[3];
boolean flag = false;
void setup()
{
  Mrif1();
  display.begin(SSD1306_SWITCHCAPVCC);                    //显示屏初始化
  pinMode(X_dir, INPUT_PULLUP);
  pinMode(X_interruptPin, INPUT_PULLUP);
  pinMode(Y_dir, INPUT_PULLUP);
  pinMode(Y_interruptpin, INPUT_PULLUP);                  //中断引脚初始化
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  Serial.begin(115200);                                 //初始化GY953,连续输出模式
}
void loop()
{
  angle();
  attachInterrupt(digitalPinToInterrupt(X_interruptPin), xblink, FALLING );
  attachInterrupt(digitalPinToInterrupt(Y_interruptpin), yblink, FALLING );//编码器中断，计数
  X_juli = 1.0 * X / 256 * 119.73;
  Y_juli = 0.5 * Y / 256 * 119.73;
  ad.realXY(X_juli, Y_juli, p1);
  data[0] =700-ad.x*0.13813;
  data[1] =372-ad.y*0.13813;
  data[2] =-ad.p;                //串口显示
  Serial.print("A:");  //串口显口
  Serial.print(0);                 //串口显示
  Serial.print("B:");  //串口显口
  Serial.print(0);                 //串口显示
  Serial.print("X:");  //串口显口
  Serial.print(data[0]);                 //串口显示
  Serial.print("Y:");                   //串口显示
  Serial.print(data[1]);                 //串口显示
  Serial.print("P:");                   //串口显示
  Serial.println(p1);  //串口显口
  delay(1);
  view(ad.x,ad.y,p1);
}
void xblink()
{
  xstate = digitalRead(X_dir);
  if (xstate == HIGH)
    X++;
  else
    X--;
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
    JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
  }
}
void Mrif1()
{
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"12345");
  Mirf.setTADDR((byte *)"67890");
  Mirf.payload = sizeof(data);
  Mirf.channel = 3;
  Mirf.config();
}
void angle()
{
  Ang[0] = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
  Ang[1] = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
  Ang[2] = (float)JY901.stcAngle.Angle[2] / 32768 * 180;
  if (!flag)
  {
    p0 = Ang[2];
  }
  if(p0!= 0)
  {
     flag = true;
    }
  p1 = Ang[2];
  ad.tozero(p1, p0);
}
void view(double x, double y, double p)
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
