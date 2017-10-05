#include "Arietta.h"

#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TCanvas.h>

#include <iostream>

using namespace std;

Arietta::Arietta(std::string name, int nBins, string filename, double maximum)
	:maximum(maximum), fileName(filename)
{
	hist = new TH1D(name.c_str(), "", nBins, 0, 10000);
}

void Arietta::sendTrig(Time muonTime, Option<Time> elecTime)
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
	auto f = TFile::Open(fileName.c_str(), "RECREATE");
	
	hist->Write();
	
	delete f;
	currentDir->cd();
	
	TCanvas c;
	hist->Draw();
	c.SaveAs("output.png");
}
