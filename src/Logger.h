#pragma once
#include <vector>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"

#include "MissionInfo.h"

class Logger { 

	static const std::string UT_FORMAT;
	static const std::string MET_FORMAT;
	static const std::string RT_FORMAT;

	static const std::string STREAM_UT_FORMAT;
	static const std::string STREAM_MET_FORMAT;

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

	template<typename... Args>
	void critical(const char* message, const Args &... args) const {
		log(spdlog::level::critical, message, args...);
	}

	template<typename... Args>
	void err(const char* message, const Args &... args) const {
		log(spdlog::level::err, message, args...);
	}

	template<typename... Args>
	void warn(const char* message, const Args &... args) const {
		log(spdlog::level::warn, message, args...);
	}

	template<typename... Args>
	void info(const char* message, const Args &... args) const {
		log(spdlog::level::info, message, args...);
	}

	template<typename... Args>
	void trace(const char* message, const Args &... args) const {
		log(spdlog::level::trace, message, args...);
	}

	template<typename... Args>
	void debug(const char* message, const Args &... args) const {
		log(spdlog::level::debug, message, args...);
	}

};

