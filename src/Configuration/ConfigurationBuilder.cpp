#include "ConfigurationBuilder.h"

#include "Configuration.h"
#include "Module.h"
#include "B.h"

#include "Objects/ActiveObject.h"

using namespace std;

ConfigurationBuilder::ConfigurationBuilder()
	:conf(new Configuration)
{
}

ConfigurationBuilder::~ConfigurationBuilder()
{
}

ConfigurationBuilder::TempTrigger ConfigurationBuilder::addTrigger(std::array<double, nMaxObjects> thresholds, std::bitset<nMaxObjects> activeCh, std::bitset<nMaxObjects> veto)
{
	auto trig = new Trigger(thresholds, activeCh, veto);
	conf->triggers.emplace_back(trig);
	
	return TempTrigger(*trig, *this);
}

std::unique_ptr<Configuration> ConfigurationBuilder::configure()
{
	return move(conf);
}

ConfigurationBuilder& ConfigurationBuilder::loadB(std::string filename, double delta)
{
	conf->bg.reset(new BGen(filename, delta));
	
	return *this;
}
