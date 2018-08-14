/* 
* qingling.cpp
*
* Created: 2018/8/11 16:38:11
* Author: Administrator
*/


#include "Suanfa.h"
#include "Arduino.h"
// default constructor
Suanfa::Suanfa()
{
} //Suanfa
void Suanfa::tozero(double p1,double p0)
{
	 p = p1 - p0;
	 if(abs(p)>=180)
	 {
		 if(p0>=0)
		 {
			 p = map(p,-180,-180-p0,180,180-p0);
		 }
		 else
		 {
			 p = map(p,180,180-p0,-180,-180-p0);
		 }
	 }
}
void Suanfa::realXY(double x1,double y1,double p1)
{
	double dx = x1 - _x1;
	double dy = y1 - _y1;
	double dp = p1 - _p1;
  dp = dp*PI/180;
  double dx1 = dx*cos(dp)-dy*sin(dp);
  double dy1 = dx*sin(dy)+dy*cos(dp);
	x += dx1;
	y += dy1;
	
	_x1 = x1;
	_y1 = y1;
	_p1 = p1;
}
