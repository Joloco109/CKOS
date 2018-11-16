#pragma once
#include "Logger.h"

class Unit {
	protected:
		Logger logger;

	public:
		Unit(std::string name);
		Unit(std::string name, std::ostream* dest);
		Unit(std::string name, std::shared_ptr<MissionInfo>);
		Unit(std::string name, std::ostream* dest, std::shared_ptr<MissionInfo>);


		const Logger& getLogger() const;
};
