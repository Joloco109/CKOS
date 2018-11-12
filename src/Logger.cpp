#include <iostream>
#include <fstream>
#include <fstream>
#include <vector>
#include <memory>
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/ostream_sink.h"

/*
 * Definition of Logger Members
 *
 */

std::vector<std::shared_ptr<spdlog::sinks::ostream_sink_mt>> Logger::m_files;

void Logger::init() {
	if (m_files.empty()) {
		auto main_log = std::ofstream("main.log");
		auto mission_log = std::ofstream("mission.log");
		auto telemetry_log = std::ofstream("telemetry.log");
		m_files.resize(3);
		m_files[OutputFiles::Main] = std::make_shared<spdlog::sinks::ostream_sink_mt>(main_log);
		m_files[OutputFiles::Mission] = std::make_shared<spdlog::sinks::ostream_sink_mt>(mission_log);
		m_files[OutputFiles::Telemetry] = std::make_shared<spdlog::sinks::ostream_sink_mt>(telemetry_log);
	}
}


Logger::Logger(const std::string& name) :
	m_name(name),
	m_logger(std::make_shared<spdlog::logger>(name, std::make_shared<spdlog::sinks::stdout_color_sink_mt>())) {
	init();
	m_logger->info("Logger {} initialized.", m_name);
}

Logger::Logger(const std::string& name, std::ostream* dest) : 
	m_name(name), 
	m_logger(std::make_shared<spdlog::logger>(name, std::make_shared<spdlog::sinks::ostream_sink_mt>(*dest)))  
{
	init();
	m_logger->info("Logger {} initialized.", m_name);
}

/*Logger::Logger(const std::string& name, OutputFiles dest) :
	m_name(name), 
	m_logger(std::make_shared<spdlog::logger>(name, m_files[dest])) 
{
	init();
	m_logger->info("Logger {} initialized.", m_name);
}*/

void Logger::addLogStream(std::ostream* dest) {
	m_logger->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(*dest));
	
}

/*void Logger::addLogStream(OutputFiles dest) {
	//m_logger->sinks().push_back(m_files[dest]);
	auto main_log = std::ofstream("main.log");
	m_logger->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(main_log));
}*/

/*template<typename... Args>
void Logger::log(const spdlog::level::level_enum level, const char* message, const Args & ... args) const {
	m_logger->log(level, message, &args...);
}*/

void Logger::log(const spdlog::level::level_enum level, const std::string& message) const {
	m_logger->log(level, message);
}

void Logger::critical(const std::string& message) {
	m_logger->critical(message);
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

/*int LogTest() {
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
}*/
