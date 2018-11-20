#pragma once
#include <thread>
#include <future>
#include <utility>

#include "MissionInfo.h"
#include "Logger.h"

class StreamLogger {
	std::fstream* m_output;
	std::shared_ptr<Logger> m_logger;
	std::thread* m_thread;
	std::promise<void> exitSignal;

	std::shared_ptr<MissionInfo> m_info;
	std::vector<std::pair<std::string, std::shared_ptr<krpc::Stream<float>>>> m_float_streams;
	std::vector<std::pair<std::string,std::shared_ptr<krpc::Stream<double>>>> m_double_streams;

	void log(std::future<void> futureSignal, int secs);

	public:
	StreamLogger(std::shared_ptr<MissionInfo> info);

	~StreamLogger();

	void add_float_stream(std::string name, std::shared_ptr<krpc::Stream<float>> float_stream);

	void add_double_stream(std::string name, std::shared_ptr<krpc::Stream<double>> double_stream);

	void log_start();

	void log_stop();
};
