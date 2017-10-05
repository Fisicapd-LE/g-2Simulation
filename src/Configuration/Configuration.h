#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <memory>
#include <vector>

#include "Option/Option.h"

#include "Utilities/channels.h"

#include "Tracks/Track.h"

class Track;
class Interaction;
class ActiveObject;
class Trigger;
class Module;
class ConfigurationBuilder;
class BGen;

class Configuration final
{
	friend ConfigurationBuilder;
	public:
		~Configuration();
	
		static std::unique_ptr<ConfigurationBuilder> create();
		
		void process(std::unique_ptr<Track>&& cosmic) const;
	
	private:
		Configuration() = default;
		Configuration(Configuration&) = delete;
		Configuration& operator=(Configuration&) = delete;
		
		void trigger(const Interaction& muonInt, const Option<Interaction>& elecInt) const;
		
		std::vector<std::unique_ptr<ActiveObject>> objects;
		std::vector<std::unique_ptr<Trigger>> triggers;
		std::vector<std::unique_ptr<Module>> modules;
		
		std::unique_ptr<BGen> bg;
		
};

#endif 
