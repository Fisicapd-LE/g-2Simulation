/*
 * Decay.h
 *
 *  Created on: 03/ago/2017
 *      Author: davide
 */


#ifndef DECAY_H_
#define DECAY_H_


class Decay
{
public:
  //   Active_Object();
  //   Active_Object(double zheight);
  //   Active_Object(double zheight, double xdisplace, double ydisplace);
  Active_Object(double zheight = 0, double xdisplace = 0, double ydisplace = 0, double xlen = 600, double ylen = 250, double zlen = 15);
  
  virtual ~Active_Object();
  
  //to read object data
  double GetZposition() const 		{return zposition;};
  double GetXlen() const 		{return xlen;};
  double GetYlen() const 		{return ylen;};
  double GetZlen() const 		{return zlen;};
  double GetXdisplace() const 		{return xdisplace;};
  double GetYdisplace() const 		{return ydisplace;};
  //   double GetZdisplace() const 		{return zdisplace;};
  double GetXentering() const 		{return Xentering_point;};
  double GetYentering() const 		{return Yentering_point;};
  double GetZentering() const 		{return Zentering_point;};
  
  void SetZposition(double z);
  
  double SpaceTravelled (const Cosmic & ray) const;	//function to compute the space cosmic ray travels inside the active object before going out
  //IT DOES NOT CONSIDERATE INTERACTION WITH MATTER
  //TODO LA PREFERIVO PROTECTED, CAMBIERO' L'ABSORBER.CPP PER FARLA TORNARE GIU'
  private:
  
  double time;
  
  
  
  
  //copied from Enrico's Cosmic.h
  static std::random_device& rd()
  {
    static std::random_device rd;
    return rd;
  };
  static std::mt19937_64& gen()
  {
    static std::mt19937_64 gen(rd()());
    return gen;
  };
  
  
};

#endif /* ACTIVE_OBJECT_H_ */ 

