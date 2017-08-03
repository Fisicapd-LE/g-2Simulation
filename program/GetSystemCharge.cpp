/*
 * GetScintCharge.cpp
 *
 *  Created on: 23/mag/2017
 *      Author: davide
 */

//Funzione per andare a stimare il segnale letto dato un evento e un sistema di scintillatori
//Possibile implementazione a classe? Ai posteri l'ardua sentenza

#include <vector>

#include "Cosmic.h"
#include "Scintillator.h"

using namespace std;

// double GetSingleCharge(const Scintillator & scint1, const Cosmic & ray);

vector<double> GetSystemCharge(const vector<Scintillator> scint_vec, const Cosmic & ray)
{
  vector<double> output;
  for(unsigned int i = 0; i < scint_vec.size(); i++) 
     output.push_back( (scint_vec.at(i)).GetSingleCharge(ray) );
  return output;  
}

