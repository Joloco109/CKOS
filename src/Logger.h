#pragma once
#include <vector>
#include <memory>

enum class LogLevel{
	Norminal,
	Warning,
	Error,
	RUD
};


class Logger { 
	struct LogDest {
		std::ostream* m_dest;
		std::vector<LogLevel> m_levels;

		LogDest(std::ostream* dest, std::vector<LogLevel> levels): m_dest(dest), m_levels(levels)  {}
	};

	std::vector<LogDest> m_logDests;

	public:
	Logger();

	Logger(std::ostream* dest, const std::vector<LogLevel>& levels);

	void addLogStream(std::ostream* dest, const std::vector<LogLevel>& levels);

	void log(const std::string& message, const LogLevel level, const std::string& unit) const;

	void log(const std::string& message, const LogLevel level) const;
};

class StreamLogger {
	std::shared_ptr <Logger> m_logger;

};

