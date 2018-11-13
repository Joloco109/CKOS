#include "CKOS_Unit.h"

Unit::Unit(std::string name) :
	logger(name)
{ }

Unit::Unit(std::string name, std::ostream* dest) :
	logger(name, dest)
{ }

const Logger& Unit::getLogger() const {
	return logger;
}
