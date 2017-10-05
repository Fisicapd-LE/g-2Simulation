#ifndef CONFIGURATION_BUILDER_H_
#define CONFIGURATION_BUILDER_H_

#include "Configuration.h"
#include "Trigger.h"

#include "Utilities/channels.h"

#include <memory>
#include <bitset>
#include <array>

class Configuration;

class ConfigurationBuilder
{
	private:
		std::unique_ptr<Configuration> conf;
		
	public:
		class TempTrigger
		{
			public:
				TempTrigger(Trigger& trigger, ConfigurationBuilder& conf) 
					:trigger(trigger), confBuild(conf)
				{
				};
			
				template <typename T, typename ...Args>
				ConfigurationBuilder& toModule(Args... args);
				
			private:
				Trigger& trigger;
				ConfigurationBuilder& confBuild;
		};
		
		ConfigurationBuilder();
		ConfigurationBuilder(const ConfigurationBuilder&) = delete;
		ConfigurationBuilder& operator=(const ConfigurationBuilder&) = delete;
		~ConfigurationBuilder();
		
		ConfigurationBuilder& startConfiguring()
		{
			return *this; // because of reasons
		}
		
		template <typename T, typename ...Args>
		ConfigurationBuilder& addObject(Args... args);
		
		TempTrigger addTrigger(std::array<double, nMaxObjects> thresholds, std::bitset<nMaxObjects> activeCh = 0x0, std::bitset<nMaxObjects> veto = 0x0);
		
		ConfigurationBuilder& loadB(std::string filename, double delta);
		
		std::unique_ptr<Configuration> configure();
		
};

template <typename T, typename ...Args>
ConfigurationBuilder& ConfigurationBuilder::addObject(Args... args)
{
	conf->objects.emplace_back(new T(args...));
	
	return *this;
}

template <typename T, typename ...Args>
ConfigurationBuilder& ConfigurationBuilder::TempTrigger::toModule(Args... args)
{
	auto mod = new T(args...);
	confBuild.conf->modules.emplace_back(mod);
	
	trigger.destination = mod;
	
	return confBuild;
}

#endif 
