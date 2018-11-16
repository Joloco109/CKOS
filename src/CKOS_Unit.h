#pragma once
#include "Logger.h"

class Unit {
	protected:
		Logger logger;

	public:
		Unit(const std::string& name);
		Unit(const std::string& name, std::ostream* dest);
		Unit(const std::string& name, std::shared_ptr<MissionInfo>);
		Unit(const std::string& name, std::ostream* dest, std::shared_ptr<MissionInfo>);


		const Logger& getLogger() const;
};
