/*
 * Scintillator.cpp
 *
 *  Created on: 23/mag/2017
 *      Author: Davide
 */

// #include "Discriminator.h"
// #include "Charge.h"



#include "Scintillator.h"
#include "Cosmic.h"

using namespace std;

Scintillator::Scintillator(double zheight, double xdisplace, double ydisplace, double xlen, double ylen, double zlen, short int mode, double width):
  Active_Object(zheight, xdisplace, ydisplace, xlen, ylen, zlen),
  mode( mode ),
  width( width )
  {
    conv_ratio = mode / width;
  }

Scintillator::~Scintillator()
  { }

void Scintillator::SetConv_ratio(double ratio)
{
  conv_ratio = ratio;
  return;
}


double Scintillator::GetSingleCharge(const Cosmic & ray) const
{
  
  double ratio = conv_ratio;
  return SpaceTravelled(ray) * conv_ratio;
}
/*  double x0,y0,phi,theta;
  x0 = ray.pos.x;
  y0 = ray.pos.y;
  phi = ray.dir.phi;
  theta = ray.dir.theta;
  double min[3], max[3];
  double x1, x2, y1, y2, z1, z2;
  
  double x = xdisplace;
  double y = ydisplace;
  double z = zposition;
  
  double dx = xlen *.5;
  double dy = ylen *.5;
  double dz = zlen *.5;
  
  double ratio = conv_ratio;
  
  //    short int moda = 8;		//numero medio fotoni visti
  //    double spessore = 10;		//spessore a cui si è visto tale numero medio di fotoni (mm)
  
  //x
  if (phi == M_PI_2 or phi == 3*M_PI_2)
  {
    if (x0 > x-dx and x0 < x+dx)
    {
      min[0] = -numeric_limits<double>::max();
      max[0] = numeric_limits<double>::max();
    }
    else
      return 0;
  }
  else
  {
    x1 = (x-x0-dx)/(cos(phi)*sin(theta));
    x2 = (x-x0+dx)/(cos(phi)*sin(theta));
    if(x1 < x2)
    {
      min[0] = x1;
      max[0] = x2;
    }
    else
    {
      min[0] = x2;
      max[0] = x1;
    }
  }
  
  //y
  if (phi == 0 or phi == M_PI)
  {
    if (y0 > y-dy and y0 < y+dy)
    {
      min[1] = -numeric_limits<double>::max();
      max[1] = numeric_limits<double>::max();
    }
    else
    {
      return 0;
    }
  }
  else
  {
    y1 = (y-y0-dy)/(sin(phi)*sin(theta));
    y2 = (y-y0+dy)/(sin(phi)*sin(theta));
    if(y1 < y2)
    {
      min[1] = y1;
      max[1] = y2;
    }
    else
    {
      min[1] = y2;
      max[1] = y1;
    }
  }
  
  //z
  z1 = (z-dz)/(cos(theta));
  z2 = (z+dz)/(cos(theta));
  if(z1 < z2)
  {
    min[2] = z1;
    max[2] = z2;
  }
  else
  {
    min[2] = z2;
    max[2] = z1;
  }
  
  double Min, Max;
  Min = min[0];
  Max = max[0];
  for(int i = 0 ; i < 3; i++)
  {
    //clog << "Min" << i << " " << min[i] << endl;
    //clog << "Max" << i << " " << max[i] << endl;
    if (Min < min[i])
      Min = min[i];
    if (Max > max[i])
      Max = max[i];
  }
  
  if (Max < Min)
    return 0;
  
  return ((Max - Min) * ratio);
}*/
