#include "Configuration.h"

#include "Objects/ActiveObject.h"

using namespace std;

void Configuration::process(std::unique_ptr<Track>&& cosmic) const
{
	auto result = interact(move(cosmic));
	
	if (result.second)
	{
		auto eResult = interact(move(result.second));
		
		trigger(result.first, eResult.first);
	}
	else
	{
		trigger(result.first, No<Interaction>());
	}
	
}

std::pair<Interaction, std::unique_ptr<Track>> Configuration::interact(std::unique_ptr<Track>&& cosmic) const
{
	auto track = unique_ptr<Track>(move(cosmic));
	Interaction inter;
	
	for (const auto& obj: objects)
	{
		track = obj->interact(track, inter);
		if (track.f >= Track::Flavour::eP)
		{
			inter.addCharge(obj.ID, 0);
			inter.addIntersectionPoint(obj.ID, No<double>(), No<double>());
		}
	}
	
	std::unique_ptr<Track> resultTrack(nullptr);
	
	if (track.f >= Track::Flavour::eP)
		resultTrack = move(track);
		
	return make_pair(inter, resultTrack);
}

Interaction Configuration::elecInteract(std::unique_ptr<Track>&& cosmic) const
{
	auto track = unique_ptr<Track>(move(cosmic));
	Interaction inter;
	
	for (const auto& obj: objects)
	{
		track = obj->interact(track, inter);
		if (!track)
		{
			inter.addCharge(obj.ID, 0);
			inter.addIntersectionPoint(obj.ID, No<double>(), No<double>());
		}
	}
	
	return inter;
}

void Configuration::trigger(const Interaction& muonInt, const Option<Interaction&> elecInt) const
{
	for (auto& trig: triggers)
	{
		trig->processEvent(muonInt, elecInt);
	}
}
		
		

