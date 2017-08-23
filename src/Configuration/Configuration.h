#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <memory>
#include <vector>

//#include "ConfigurationBuilder.h"

#include "Utilities/Option.h"

class Track;
class Interaction;
class ActiveObject;

class Configuration
{
	//friend ConfigurationBuilder;
	public:
		//static ConfigurationBuilder new();
		
		void process(std::unique_ptr<Track>&& cosmic) const;
		
		static constexpr int nMaxObjects = 16;	
	
	private:
		Configuration() = default;
		Configuration(Configuration&) = delete;
		Configuration& operator=(Configuration&) = delete;
		
		class Trigger;
		class Module;
		
		std::pair<Interaction, std::unique_ptr<Track>> muonInteract(std::unique_ptr<Track>&& cosmic) const;
		Interaction elecInteract(std::unique_ptr<Track>&& cosmic) const;
		void trigger(const Interaction& muonInt, const Option<Interaction&> elecInt);
		
		std::vector<std::unique_ptr<ActiveObject>> objects;
		std::vector<std::unique_ptr<Trigger>> triggers;
		std::vector<std::unique_ptr<Module>> modules;
		
};

#endif 
