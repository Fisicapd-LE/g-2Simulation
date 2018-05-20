/*
 * Main.cpp
 *
 *  Created on: 05/lug/2016
 *      Author: enrico, enhanced by raffaele and davide
 */

#include "Args/AnalisysInfo.h"

#include "Utilities/Vector3D.h"

#include "Tracks/Track.h"

#include "Objects/Scintillator.h"
#include "Objects/Absorber.h"

#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationBuilder.h"
#include "Configuration/Arietta.h"
#include "Configuration/Trigger.h"
#include "Configuration/Arietta.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <TH1.h>
#include <TStyle.h>

#include <ROOT/TThreadExecutor.hxx>

using namespace std;

int main(int argc, char** argv)
{
	ROOT::EnableThreadSafety();
	TH1::AddDirectory(false);
	gStyle->SetOptStat(1111111);

	unique_ptr<AnalysisInfo> info(new AnalysisInfo(argc, argv));  //argomenti da riga di comando
	
	UChar_t nThreads = 1;
	if (info->contains("-j"))	// se hai passato il numero di eventi come parametro
	{
		nThreads = stoi(info->value("-j"));	// leggi il parametro e usalo
	}
	
	long  nEvent = 100000;
	if (info->contains("-n"))	// se hai passato il numero di eventi come parametro
	{
		nEvent = stol(info->value("-n"));	// leggi il parametro e usalo
	}
	
	string filename = info->contains("-B")?info->value("-B"):"#simple";
	
	string cleanname = filename;
	replace( cleanname.begin(), cleanname.end(), '#', '_');
	string outfile = "output_" + to_string(nEvent) + "_" + cleanname + ".root";
	clog << outfile << endl;
	if (info->contains("-o"))	// se hai passato il numero di eventi come parametro
	{
		outfile = info->value("-o");	// leggi il parametro e usalo
	}

	clog << "Configuring\n";
	const auto conf = Configuration::create()->startConfiguring()
		//.addObject<Scintillator>(-5.1)
		.addObject<Scintillator>(-18.8)
		.addObject<Scintillator>(-32.5)
		.addObject<Scintillator>(-72.5)
		.addObject<Absorber>(-52.5)
		.addTrigger(
			{
				3.5, 
				3.5, 
				3.5,
			}, 
			0x7, 
			0x4
		)
			.toModule<Arietta>("output", info->contains("-N")?stoi(info->value("-N")):625, outfile)
		.loadB(filename, 2.5)
		.configure();
	
	
	int interval = nEvent/20;
	
	ROOT::TThreadExecutor pool(nThreads);

	clog << "Starting loop\n";
	
	atomic<long> counter(0);
	
	// ah, the things you have to do because of a 4 byte integer...
	for(int i = 0; i < nEvent/1000000000; i++)
	{
		clog << i * 1000000000L << endl;
		pool.Foreach(
			[&conf, interval, &counter]()
			{
				auto event = Track::generate();			//generazione evento

				conf->process(move(event));
			
				counter++;
			},
			1000000000
		);
	}
	pool.Foreach(
		[&conf, interval, &counter]()
		{
			auto event = Track::generate();			//generazione evento

			conf->process(move(event));
		
			counter++;
		},
		nEvent%1000000000
	);
	
	clog << "Processed " << counter << " events" << endl;

	return 0;
}

