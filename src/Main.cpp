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

#include <TH1.h>

#include <ROOT/TThreadExecutor.hxx>

using namespace std;

int main(int argc, char** argv)
{
	ROOT::EnableThreadSafety();
	TH1::AddDirectory(false);

	unique_ptr<AnalysisInfo> info(new AnalysisInfo(argc, argv));  //argomenti da riga di comando
	
	UChar_t nThreads = 4;
	if (info->contains("-j"))	// se hai passato il numero di eventi come parametro
	{
		nThreads = stoi(info->value("-j"));	// leggi il parametro e usalo
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
			.toModule<Arietta>("output", info->contains("-N")?stoi(info->value("-N")):625, info->contains("-o")?info->value("-o"):"output.root", nThreads)
		.loadB(info->contains("-B")?info->value("-B"):"#simple", 2.5)
		.configure();
			
	long  nEvent = 100000;
	if (info->contains("-n"))	// se hai passato il numero di eventi come parametro
	{
		nEvent = stol(info->value("-n"));	// leggi il parametro e usalo
	}
	
	int interval = nEvent/20;
	
	ROOT::TThreadExecutor pool(nThreads);

	clog << "Starting loop\n";
	
	pool.Foreach(
		[&conf, interval](int sim)
		{
			auto event = Track::generate();			//generazione evento

			conf->process(move(event));
		
			if (sim%interval == 0)
			{
				clog << sim << endl;
			}
		},
		ROOT::TSeqL(nEvent)
	);

	return 0;
}

