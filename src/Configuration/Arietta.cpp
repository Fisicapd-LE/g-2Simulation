#include "Arietta.h"

#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>

#include <iostream>

using namespace std;

Arietta::Arietta(std::string name, double maximum)
	:maximum(maximum)
{
	hist = new TH1D(name.c_str(), "", 624, 8, 10000);
}

void Arietta::sendTrig(time muonTime, Option<time> elecTime)
{
	if (!elecTime)
	{
		hist->Fill(maximum);
		//clog << "Maximum\n";
		return;
	}
	
	//clog << "arietta: " << *elecTime - muonTime << endl;
		
	hist->Fill(*elecTime - muonTime);
}

Arietta::~Arietta()
{
	clog << "Deleting Arietta\n";
	TDirectory* currentDir = gDirectory;
	auto f = TFile::Open("test.root", "RECREATE");
	
	hist->Write();
	
	delete f;
	currentDir->cd();
}
