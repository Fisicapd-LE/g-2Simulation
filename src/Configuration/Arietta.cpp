#include "Arietta.h"

#include <TFile.h>
#include <TDirectory.h>
#include <TCanvas.h>

#include <ROOT/TSeq.hxx>

#include <iostream>

using namespace std;

Arietta::Arietta(std::string name, int nBins, string filename, double maximum)
	:maximum(maximum), hist(name.c_str(), "", nBins, 0, maximum), fileName(filename)
{
}

void Arietta::sendTrig(Time muonTime, Option<Time> elecTime)
{
	auto thisHist = hist.Get();
	if (!elecTime)
	{
		thisHist->Fill(maximum);
		//clog << "Maximum\n";
		return;
	}
	
	//cout << *elecTime - muonTime << endl;
		
	thisHist->Fill(*elecTime - muonTime);
}

Arietta::~Arietta()
{
	clog << "Deleting Arietta\n";
	TDirectory* currentDir = gDirectory;
	auto f = TFile::Open(fileName.c_str(), "RECREATE");
	
	auto mergedHist = hist.Merge();
	
	mergedHist->Write();
	
	delete f;
	currentDir->cd();
	
	TCanvas c;
	mergedHist->Draw();
	c.SaveAs("output.png");
}
