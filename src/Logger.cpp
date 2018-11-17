#include "Logger.h"

#include <iostream>
#include <fstream>
#include <fstream>
#include <vector>
#include <memory>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/ostream_sink.h"

#include "krpc.hpp"
#include "krpc/services/krpc.hpp"

#include "krpc_formatter.h"


/*
 * Definition of Logger Members
 *
 */

const std::string Logger::UT_FORMAT = "[%U] [%^%n%$] [%l]  %v";
const std::string Logger::MET_FORMAT = "[%Z] [%^%n%$] [%l]  %v";
const std::string Logger::RT_FORMAT = "[RT: %Y-%m-%d %H:%M:%S] [%^%n%$] [%l]  %v";

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
	if (m_info)
		for(auto& sink : m_logger->sinks()) {
			sink->set_formatter(spdlog::details::make_unique<spdlog::krpc_pattern_formatter>(UT_FORMAT, m_info->ut, m_info->met));
		}
	else 
		for (auto& sink : m_logger->sinks()) {
			sink->set_formatter(spdlog::details::make_unique<spdlog::krpc_pattern_formatter>(RT_FORMAT));
		}
}


Logger::Logger(const std::string& name, std::shared_ptr<MissionInfo> info) :
	m_name(name),
	m_logger(std::make_shared<spdlog::logger>(
		name,
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
		)),
	m_info(info)
{
	init();
	m_logger->info("Logger {} initialized.", m_name);
}

Logger::Logger(const std::string& name, std::ostream* dest, std::shared_ptr<MissionInfo> info) : 
	m_name(name), 
	m_logger(std::make_shared<spdlog::logger>(
		name,
		std::make_shared<spdlog::sinks::ostream_sink_mt>(*dest))),
	m_info(info)
{
	init();
	m_logger->info("Logger {} initialized.", m_name);
}

/*Logger::Logger(const std::string& name, OutputFiles dest, std::shared_ptr<MissionInfo>) :
	m_name(name), 
	m_logger(std::make_shared<spdlog::logger>(name, m_files[dest])) 
{
	init();
	m_logger->info("Logger {} initialized.", m_name);
}*/

void Logger::setInfo(std::shared_ptr<MissionInfo> info) {
	m_info = info;
	init();
}

void Logger::addLogStream(std::ostream* dest) {
	m_logger->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(*dest));
	
}

/*void Logger::addLogStream(OutputFiles dest) {
	//m_logger->sinks().push_back(m_files[dest]);
	auto main_log = std::ofstream("main.log");
	m_logger->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(main_log));
}*/

void Logger::log(const spdlog::level::level_enum level, const std::string& message) const {
	m_logger->log(level, message);
}

/*void Logger::critical(const std::string& message) const {
	m_logger->critical(message);
}*/



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
