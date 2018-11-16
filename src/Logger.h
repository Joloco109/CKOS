#pragma once
#include <vector>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"

#include "MissionInfo.h"

class Logger { 

	std::string m_name;

	std::shared_ptr<spdlog::logger> m_logger;

	std::shared_ptr<MissionInfo> m_info;

	static std::vector<std::shared_ptr<spdlog::sinks::ostream_sink_mt>> m_files;

	void init();

	public:

	enum OutputFiles {
		Main = 0,
		Mission = 1,
		Telemetry = 2
	};

	Logger(const std::string& name, std::shared_ptr<MissionInfo> = std::shared_ptr<MissionInfo>(NULL));

	Logger(const std::string& name, std::ostream* dest, std::shared_ptr<MissionInfo> = std::shared_ptr<MissionInfo>(NULL));

	//Logger(const std::string& name, OutputFiles dest, std::shared_ptr<MissionInfo> = std::shared_ptr<MissionInfo>(NULL));

	//Logger(const std::string& name, std::vector<std::ostream*> dest, std::vector<OutputFiles> destFiles, std::shared_ptr<MissionInfo> = std::shared_ptr<MissionInfo>(NULL));
	
	void setInfo(std::shared_ptr<MissionInfo> info);

	void addLogStream(std::ostream* dest);

	//void addLogStream(OutputFiles dest);

	template<typename... Args>
	void log(const spdlog::level::level_enum level, const char* message, const Args &... args) const {
		m_logger->log(level, message, args...);
	}

	void log(const spdlog::level::level_enum level, const std::string& message) const;

	void critical(const std::string& message);

};

class StreamLogger {
	std::shared_ptr <Logger> m_logger;

};

