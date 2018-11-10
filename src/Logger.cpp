#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include "Logger.h"

/*
 * Definition of Logger Members
 *
 */

Logger::Logger() {
	LogLevel defaultLevels [] = {LogLevel::Norminal, LogLevel::Warning, LogLevel::Error, LogLevel::RUD};
	m_logDests.push_back(LogDest(&std::cout, std::vector<LogLevel>(defaultLevels, defaultLevels+sizeof(defaultLevels)/sizeof(LogLevel))));
}

Logger::Logger(std::ostream* dest, const std::vector<LogLevel>& levels) {
	m_logDests.push_back(LogDest(dest, levels));
}

void Logger::addLogStream(std::ostream* dest, const std::vector<LogLevel>& levels) {
	m_logDests.push_back(LogDest(dest, levels));
}

void Logger::log(const std::string& message, const LogLevel level, const std::string& unit) const {
	for (LogDest dest : m_logDests) {
		if (std::find(dest.m_levels.begin(), dest.m_levels.end(), level) != dest.m_levels.end())
			*dest.m_dest << "[" << unit << "] " << message << std::endl;
	}
}


void Logger::log(const std::string& message, const LogLevel level) const {
	for (LogDest dest : m_logDests) {
		if (std::find(dest.m_levels.begin(), dest.m_levels.end(), level) != dest.m_levels.end())
			*dest.m_dest  << message << std::endl;
	}
}

/*
 * Definition of StreamLogger members
 *
 */

/*class StreamLogger {
	std::shared_ptr <Logger> logger;

	struct StreamNote {
		//krpc::Stream stream;
	};

	
};*/

int LogTest() {
	Logger tester;
	
	LogLevel levels [] = {LogLevel::RUD, LogLevel::Error};

	std::fstream file("test2.log", std::fstream::out);

	Logger tester2(&file, std::vector<LogLevel>(levels, levels+sizeof(levels)/sizeof(LogLevel)) );

	tester.log("Test Norminal",LogLevel::Norminal);
	tester.log("Test Warning",LogLevel::Warning);
	tester.log("Test Error",LogLevel::Error);
	tester.log("Test RUD",LogLevel::RUD);

	tester2.log("Test Norminal",LogLevel::Norminal);
	tester2.log("Test Warning",LogLevel::Warning);
	tester2.log("Test Error",LogLevel::Error);
	tester2.log("Test RUD",LogLevel::RUD);

	file.close();

	return 0;
}
