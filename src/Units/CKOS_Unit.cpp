#include "CKOS_Unit.h"

Unit::Unit(const std::string& name) :
	logger(name)
{ }

Unit::Unit(const std::string& name, std::ostream* dest) :
	logger(name, dest)
{ }

Unit::Unit(const std::string& name, std::shared_ptr<MissionInfo> info) :
	logger(name, info)
{ }

Unit::Unit(const std::string& name, std::ostream* dest, std::shared_ptr<MissionInfo> info) :
	logger(name, dest, info)
{ }

const Logger& Unit::getLogger() const {
	return logger;
}
