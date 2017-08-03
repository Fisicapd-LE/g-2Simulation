/*
 * Main.cpp
 *
 *  Created on: 05/lug/2016
 *      Author: enrico, enhanced by raffaele and davide
 */

#include "Args/AnalisysInfo.h"

#include <iostream>
//#include <fstream>
#include <vector>

#include "Cosmic.h"
#include "Scintillator.h"
#include "Absorber.h"
#include "Position3D.h"


using namespace std;

short Configuration(const vector<double> & total_charge);
vector<double> GetSystemCharge(const vector<Scintillator> scint_vec, const Cosmic & ray);
Position3D * Absorbing_ornot(const Absorber * abs0, const Cosmic & ray);

int main(int argc, char** argv)
{
  
  
  unique_ptr<AnalysisInfo> info(new AnalysisInfo(argc, argv));  //argomenti da riga di comando
  
  //crea gli scintillatori e gli assorbitori
  vector<Scintillator> scint_vec;		//vettore di scintillatori
  vector<Absorber>     abs_vec;			//vettore di assorbitori
  
  //crea i singoli scintillatori dimensioni e rate preimpostati, si possono modificare
  Scintillator *scint0 = new Scintillator(6.9);				//
  Scintillator *scint1 = new Scintillator(46.9);			//
  Scintillator *scint2 = new Scintillator(60.6);			//
  Scintillator *scint3 = new Scintillator(74.3);			//

  //crea i singoli assorbitori di dimensioni e probabilità preimpsotate, si possono modificare
  Absorber *abs0 = new Absorber(100);
  
  
  //aggiunge gli scintillatori e gli assorbitori al vettore
  scint_vec.push_back(*scint0);
  scint_vec.push_back(*scint1);
  scint_vec.push_back(*scint2);
  scint_vec.push_back(*scint3);
  
  abs_vec.push_back(*abs0);
  
  long accepted = 0;			//numero di eventi accettati
  long signals = 0;			//numero di eventi visti dallo scintillatore analizzato
  long absorbed = 0;			//numero di eventi assorbiti
  
  long  nEvent = 100000;
  if (info->contains("-n"))	// se hai passato il numero di eventi come parametro
  {
    nEvent = stol(info->value("-n"));	// leggi il parametro e usalo
  }
  
  unique_ptr<Cosmic> event;
//   vector<Charge> Ionization;
//   vector<double> UpEvent;
//   vector<double> DownEvent;

  for (long sim = 0; sim < nEvent; sim++)	//loop sulla generazione di eventi
  {
    event = Cosmic::generate();			//generazione evento


    /*
     *FIRST TRIGGERING: per studiare l'efficienza del singolo scintillatore si triggera su un certo numero di scintillatori e si guarda 
     *se quello studiato ha visto l'evento o meno. Per cambiare quale scintillatore si guarda e su quale si triggera cambiare il file
     *configuration.cpp
     * NOTA: non si tiene conto dell'assorbitore in questo primo trigger
     */
    
    vector<double> charge = GetSystemCharge(scint_vec, *event);
    switch(Configuration(charge))
    {
       //case 0: continue;
       case 1: 
         accepted ++;
         break;
       case 2:
         accepted++;
         signals ++;
         break;
    }
    /*ABSORBING: per studiare se l'evento viene o meno assorbito dall'assorbitore*/
    Position3D * where_absorbed = Absorbing_ornot(abs0, *event);
    if(where_absorbed == nullptr) continue;	//se non assorbito non è di interesse per quest'analisi
    absorbed++;
    
    double abs_newupperheight = abs0->GetZlen() - (where_absorbed->z - abs0->GetZposition());	//quanto assorbitore rimane sopra l'evento
    double abs_newlowerheight = abs0->GetZlen() - abs_newupperheight;
    
//     Absorber *fakeabsUP = new Absorber();
    
  }
  
  cout << "Signal: " <<signals  << endl
       << "Accepted rate: " <<signals*1.0/accepted << endl
       << "Triggered: " << accepted << endl
       << "Absorbed : " << absorbed << endl
       << "Absorbed rate : " << absorbed*1.0/accepted << endl;
//        << "Boh: " << accepted*1.0/nEvent << endl;
       
       
       
       
       
       
       
       
       
  /*const int NBINS = 1201;
  int * binsUp = new int[NBINS];
  int * binsDown = new int[NBINS];
  for (int i = 0; i < NBINS; i++)	
  {
    binsUp[i] = 0;
    binsDown[i] = 0;
  }
  const double binsize = 1.0;
  for (unsigned int i = 0; i < UpEvent.size(); i++)
  {
    int binposUp = int((UpEvent[i]+600)/binsize);
    int binposDown = int((DownEvent[i]+600)/binsize);
    binsUp[binposUp]++;
    binsDown[binposDown]++;
  }
  ofstream UpFile("UpHistogram.txt");
  ofstream DownFile("DownHistogram.txt");
  for (int i = 0; i < NBINS; i++)
  {
    UpFile << binsUp[i] << '\n';
    DownFile << binsDown[i] << '\n';
  }*/
  return 0;
}

