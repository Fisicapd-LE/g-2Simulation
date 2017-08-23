#ifndef MODULE_H_
#define MODULE_H_

#include "Configuration.h"

class Configuration::Module
{
	using time = long;
	virtual void process(time t) = 0;
	
};

#endif
