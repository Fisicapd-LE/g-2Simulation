#ifndef ARIETTA_H_
#define ARIETTA_H_

#include "Module.h"

#include <string>
#include <vector>

#include <ROOT/TThreadedObject.hxx>

#include <TH1.h>

class Arietta: public Module
{
	public:
		Arietta(std::string name, int nBins, std::string filename, double maximum = 10000);
		Arietta(Arietta&) = delete;
		Arietta& operator=(Arietta&) = delete;
		
		~Arietta();
		
		virtual void sendTrig(Time muonTime, Option<Time> elecTime) override;
		
	private:
		const double maximum;
		ROOT::TThreadedObject<TH1D> hist;
		
		std::string fileName;
};

#endif
