#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "Option/Option.h"

#include "Utilities/channels.h"

#include <bitset>
#include <array>

class ConfigurationBuilder;
class Interaction;
class Module;

class Trigger final
{
	friend ConfigurationBuilder;
	
	using ChBit = std::bitset<nMaxObjects>;
	
	template <typename T>
	using ChArray = std::array<T, nMaxObjects>;
	
	private:		
		using time = long;
		
		Trigger(ChArray<double> thresholds, ChBit activeCh = 0x0, ChBit veto = 0x0);
	
		Module* destination;
	
		Option<time> processInteraction(const Interaction& inter) const; 
		
		const ChArray<double> thresholds;
		const ChBit mask;
		const ChBit veto;
	
	public:
		
		void processEvent(const Interaction& muonInt, const Option<Interaction>& elecInt);
		
};

#endif
