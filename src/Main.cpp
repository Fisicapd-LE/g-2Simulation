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

using namespace std;

int main(int argc, char** argv)
{
	unique_ptr<AnalysisInfo> info(new AnalysisInfo(argc, argv));  //argomenti da riga di comando

	clog << "Configuring\n";
	auto conf = Configuration::create()->startConfiguring()
					.addObject<Scintillator>(6.9)
					.addObject<Scintillator>(46.9)
					.addObject<Scintillator>(60.6)
					.addObject<Scintillator>(74.3)
					.addObject<Absorber>(100)
					.addTrigger({3.5, 3.5, 3.5, 3.5}, 0xf)
						.toModule<Arietta>("output")
					.configure();
			
	long  nEvent = 100000;
	if (info->contains("-n"))	// se hai passato il numero di eventi come parametro
	{
		nEvent = stol(info->value("-n"));	// leggi il parametro e usalo
	}

	unique_ptr<Track> event;

	clog << "Starting loop\n";
	for (long sim = 0; sim < nEvent; sim++)	//loop sulla generazione di eventi
	{
		event = Track::generate();			//generazione evento

		conf->process(move(event));

	}

	return 0;
}

