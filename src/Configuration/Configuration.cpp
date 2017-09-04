#include "Configuration.h"

#include "ConfigurationBuilder.h"
#include "Trigger.h"
#include "Module.h"

#include "Interaction/Interaction.h"

#include "Objects/ActiveObject.h"

#include <iostream>

using namespace std;

void Configuration::process(std::unique_ptr<Track>&& cosmic) const
{
	Interaction muonInt(std::move(cosmic), objects);
	
	muonInt.runInteraction();
	
	auto elec = muonInt.getDecay();
	
	auto elecInt = No<Interaction>();
	
	if (elec)
	{
		elecInt = Interaction(move(elec), objects);
		elecInt->runInteraction();
	}

	trigger(muonInt, elecInt);
	
}

void Configuration::trigger(const Interaction& muonInt, const Option<Interaction>& elecInt) const
{
	for (auto& trig: triggers)
	{
		trig->processEvent(muonInt, elecInt);
	}
}

Configuration::~Configuration()
{
	clog << "Deleting conf\n";
}

unique_ptr<ConfigurationBuilder> Configuration::create()
{
	return unique_ptr<ConfigurationBuilder>(new ConfigurationBuilder);
}
		
		

