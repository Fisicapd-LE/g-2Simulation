#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "Configuration.h"

class Configuration::Trigger final
{
	friend ConfigurationBuilder;
	
	private:		
		using time = long;
	
		Module& destination;
	
		Option<time> processInteraction(const Interaction& inter) const; 
	
	public:
		 void processEvent(const Interaction& muonInt, const Option<Interaction&> elecInt);
		
};

#endif
