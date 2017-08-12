#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <memory>
#include <vector>

class Track;

class Configuration
{
	public:
		void process(std::unique_ptr<Track>&& cosmic);
	
	private:
		std::vector<std::unique_ptr<ActiveObject>> objects;
};

#endif 
