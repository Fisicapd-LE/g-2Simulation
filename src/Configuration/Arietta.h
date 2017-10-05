#ifndef ARIETTA_H_
#define ARIETTA_H_

#include "Module.h"

#include <string>

class TH1D;

class Arietta: public Module
{
	public:
		Arietta(std::string name, int nBins, std::string filename, double maximum = 10000);
		Arietta(Arietta&) = delete;
		Arietta& operator=(Arietta&) = delete;
		
		~Arietta();
		
		virtual void sendTrig(Time muonTime, Option<Time> elecTime) override;
		
	private:
		TH1D* hist;
		const double maximum;
		
		std::string fileName;
};

#endif
