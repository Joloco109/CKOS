#include <fstream>
#include "StreamLogger.h"

void StreamLogger::log(std::future<void> futureSignal, int secs) {
	m_logger->info("Start streams");
	while (futureSignal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
		for (auto& data_stream : m_float_streams)
			m_logger->info("{}: {:3.2f}", 
					std::get<0>(data_stream), 
					std::get<1>(data_stream)->operator()());
		for (auto& data_stream : m_double_streams)
			m_logger->info("{}: {:3.2f}", 
					std::get<0>(data_stream), 
					std::get<1>(data_stream)->operator()());
		std::this_thread::sleep_for(std::chrono::seconds(secs));
	}
	m_logger->info("Stopped streams");
}

StreamLogger::StreamLogger(std::shared_ptr<MissionInfo> info)
	: m_output(new std::fstream("telemetry.log"))
	, m_logger(std::make_shared<Logger>(
				"Telemetry",
				m_output,
			       	info))
	, m_info(info)
{
	add_double_stream("Altitude", info->altitude);
	add_float_stream("Pitch", info->pitch);
	add_float_stream("Heading", info->heading);
	add_float_stream("Roll", info->roll);
	add_double_stream("O. Speed", info->orbit_speed);
	add_double_stream("S. Speed", info->surface_speed);
}

StreamLogger::~StreamLogger() {
	exitSignal.set_value();
	if (m_thread->joinable())
		m_thread->join();
	delete m_thread;
	delete m_output;
}

void StreamLogger::add_float_stream(std::string name, std::shared_ptr<krpc::Stream<float>> float_stream) {
	m_float_streams.push_back(std::pair<std::string, std::shared_ptr<krpc::Stream<float>>>(name, float_stream));
}

void StreamLogger::add_double_stream(std::string name, std::shared_ptr<krpc::Stream<double>> double_stream) {
	m_double_streams.push_back(std::pair<std::string, std::shared_ptr<krpc::Stream<double>>>(name, double_stream));
}

void StreamLogger::log_start() {
	exitSignal = std::promise<void>();
	auto futureSignal = exitSignal.get_future();
	m_thread = new std::thread(&StreamLogger::log, this, std::move(futureSignal),1);
	//m_thread = new std::thread([] {std::cout << "Test" << std::endl; });
}

void StreamLogger::log_stop() {
	exitSignal.set_value();
	delete m_thread;
}
