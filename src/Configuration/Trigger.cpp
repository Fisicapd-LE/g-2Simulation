#include "Trigger.h"

Configuration::Trigger::TriggerObject::TriggerObject(Trigger trig)
	:muon(0), elec(0), trig(trig)
{
}

void Configuration::Trigger::processEvent(const Interaction& muonInt, const Option<Interaction&> elecInt)
{
	auto muonTime = processInteraction(muonInt);
	
	if (!muonTime)
		return;
		
	Option<time> elecTime;
	
	if (elecInt)
		elecTime = processInteraction(*elecInt);
	
	destination.sendTrig(*muonTime, elecTime);
}
