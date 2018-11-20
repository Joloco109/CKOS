#include "StreamLogger.h"

void StreamLogger::log(std::future<bool> futureSignal, int secs) {
	m_logger->info("Start streams");
	while (!futureSignal.get()) {
		for (auto& data_stream : m_float_streams)
			m_logger->info("Data {:3.2f}", data_stream->operator()());
		for (auto& data_stream : m_double_streams)
			m_logger->info("Data {:3.2f}", data_stream->operator()());
		std::this_thread::sleep_for(std::chrono::seconds(secs));
	}
	m_logger->info("Stopped streams");
}

StreamLogger::StreamLogger(std::shared_ptr<Logger> logger)
	: m_logger(logger)
{ }

StreamLogger::~StreamLogger() {
	exitSignal.set_value(true);
	if (m_thread->joinable())
		m_thread->join();
	delete m_thread;
}

void StreamLogger::add_float_stream(std::shared_ptr<krpc::Stream<float>> float_stream) {
	m_float_streams.push_back(float_stream);
}

void StreamLogger::add_double_stream(std::shared_ptr<krpc::Stream<double>> double_stream) {
	m_double_streams.push_back(double_stream);
}

void StreamLogger::log_start() {
	exitSignal = std::promise<bool>();
	auto futureSignal = exitSignal.get_future();
	m_thread = new std::thread(&StreamLogger::log, this, std::move(futureSignal),1);
	//m_thread = new std::thread([] {std::cout << "Test" << std::endl; });
}

void StreamLogger::log_stop() {
	exitSignal.set_value(false);
	delete m_thread;
}
