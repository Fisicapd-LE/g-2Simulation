/*
 * Configuration.cpp
 *
 *  Created on: 24/mag/2017
 *      Author: davide and enrico
 */

#include <vector>
//#include <iostream>

using namespace std;

bool Triggered(const vector<double> & total_charge, const double soglia);
bool Seen(const double total_charge, const double soglia);


short Configuration(const vector <double> & total_charge)
{
   //funzione per decidere se va bene l'evento o meno
   //l'output e' zero se l'evento non viene nemmeno triggerato, uno se viene triggerato, due se viene triggerato e rivelato

   const short eff_study = 3;
   const double soglia_study = 1.3;
   const double soglia_trigger = 3.3;


   bool triggered, seen;
   short result;
   triggered = Triggered(total_charge, soglia_trigger);
   seen = Seen(total_charge.at(eff_study), soglia_study);
   if     (triggered == true && seen == true) {result = 2;}
   else {
           if(triggered == true && seen != true) {result = 1;}
           else result = 0;
         }

   //if (triggered == true) clog << total_charge.at(eff_study) << " ";
   return result;
}

bool Triggered(const vector <double> & total_charge, const double soglia)
{
   bool result;
   vector<short> trigger_list = {0,1,2};
   for(unsigned int i = 0; i < trigger_list.size(); i++) 
      if( !Seen(total_charge.at(trigger_list.at(i)), soglia) )
         return false;
   return true;
}

bool Seen(const double single_charge,const double soglia)
{
   return single_charge>soglia?true:false;
}
