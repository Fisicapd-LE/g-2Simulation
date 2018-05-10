#include "Trigger.h"

#include "Module.h"

#include "Interaction/Interaction.h"

#include <iostream>

using namespace std;

Trigger::Trigger(ChArray<double> thresholds, ChBit activeCh, ChBit veto)
	:thresholds(thresholds), mask(activeCh.flip()), veto(veto)
{
}

void Trigger::processEvent(const Interaction& muonInt, const Option<Interaction>& elecInt)
{
	auto muonTime = processInteraction(muonInt);
	
	if (!muonTime)
	{
		return;
	}
		
	Option<Time> elecTime;
	
	if (elecInt)
	{
		elecTime = processInteraction(*elecInt);
	}
	
	destination->sendTrig(*muonTime, elecTime);
}

Option<Time> Trigger::processInteraction(const Interaction& inter) const
{
	ChBit signal;
	for (int ch = 0; ch < nMaxObjects; ch++)
	{
		//clog << ch << " " << inter.getCharge(ch) << " " << thresholds[ch] << endl;
		if (inter.getCharge(ch) > thresholds[ch])
		{
			signal.set(ch);
		}
		//cout << signal[ch] << " ";
	}
	//cout << endl;
	
	ChBit chOut = mask | (signal ^ veto);
	
	bool triggered = chOut.all();
	
	//clog << "Triggered? " << triggered << endl;
	
	if (triggered)
		return inter.getTime();
		
	return No<Time>();
}
