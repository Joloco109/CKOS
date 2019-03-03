#pragma once
#include <vector>
#include <memory>
#include "fmt/fmt.h"

#include "MissionInfo.h"

namespace spdlog {
	class logger;
	namespace sinks {
		template <typename Mutex>
		class ostream_sink;

		using ostream_sink_mt = ostream_sink<std::mutex>;
	}
	namespace level {
	}
}

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

	void critical(const std::string message) const;

	template<typename... Args>
	void critical(const std::string& message, const Args &... args) const {
		critical(fmt::format(message, args...));
	}

	void err(const std::string message) const;

	template<typename... Args>
	void err(const std::string& message, const Args &... args) const {
		err(fmt::format(message, args...));
	}

	void warn(const std::string message) const;

	template<typename... Args>
	void warn(const std::string& message, const Args &... args) const {
		warn(fmt::format(message, args...));
	}

	void info(const std::string message) const;

	template<typename... Args>
	void info(const std::string& message, const Args &... args) const {
		info(fmt::format(message, args...));
	}

	void trace(const std::string message) const;

	template<typename... Args>
	void trace(const std::string& message, const Args &... args) const {
		trace(fmt::format(message, args...));
	}

	void debug(const std::string message) const;

	template<typename... Args>
	void debug(const std::string& message, const Args &... args) const {
		debug(fmt::format(message, args...));
	}
};
