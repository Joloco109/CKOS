#pragma once
#include <thread>
#include <future>

#include "MissionInfo.h"
#include "Logger.h"

class StreamLogger {
	std::shared_ptr<Logger> m_logger;
	std::thread* m_thread;
	std::promise<bool> exitSignal;

	std::vector<std::shared_ptr<krpc::Stream<float>>> m_float_streams;
	std::vector<std::shared_ptr<krpc::Stream<double>>> m_double_streams;

	void log(std::future<bool> futureSignal, int secs);

	public:
	StreamLogger(std::shared_ptr<Logger> logger);

	~StreamLogger();

	void add_float_stream(std::shared_ptr<krpc::Stream<float>> float_stream);

	void add_double_stream(std::shared_ptr<krpc::Stream<double>> double_stream);

	void log_start();

	void log_stop();
};
