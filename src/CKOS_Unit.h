#pragma once
#include "Logger.h"

class Unit {
	protected:
		Logger logger;

	public:
		Unit(std::string name);
		Unit(std::string name, std::ostream* dest);

		const Logger& getLogger() const;
};
