#include "MissionStage.h"

/*
 * Definitions of MissionStage
 *
 */

MissionStage::MissionStage(std::string name, std::shared_ptr<MissionInfo> info):
	Unit(name, info),
	m_info(info),
	m_name(name) 
{
	//logger.setInfo(m_info);
	logger.log(spdlog::level::info, "MissionStage {} created", m_name);
	//std::cout << "MissionStage " << m_name << " created!" << std::endl;
}

std::shared_ptr<MissionInfo> MissionStage::getInfo() const {
	return m_info;
}

std::string MissionStage::getName() const {
	return m_name;
}

MissionStageStatus MissionStage::getStatus() const {
	return m_status;
}

bool MissionStage::activate() {
	if (m_status != MissionStageStatus::InProgress) {
		m_status = MissionStageStatus::InProgress;
		logger.log(spdlog::level::info, "Activated Stage: {}", m_name);
		//std::cout << "Activated Stage: " << m_name << std::endl;
		return true;
	}
	return false;
}

/*
 * Definitions of CountdownStage
 *
 */

CountdownStage::CountdownStage(std::string name, std::shared_ptr<MissionInfo> info) :
	MissionStage(name, info)
{}

CountdownStage::CountdownStage(std::string name, std::shared_ptr<MissionInfo> info, double seconds) :
	MissionStage(name, info),
	m_seconds(seconds) 
{}

MissionStageStatus CountdownStage::update() {
	if (activate()) {
		m_launchtime = m_seconds + getInfo()->ut->operator()();
		logger.log(spdlog::level::info, "Launch at {} ", m_launchtime);
		//std::cout << "Launch at " << m_launchtime << std::endl;
	}
	double time = getInfo()->ut->operator()();
	if (m_launchtime - m_seconds < time) {
		if (m_seconds > 0) {
			std::cout << "\tT-" << m_seconds << std::endl;
			m_seconds--;
		}
		else {
			std::cout << "\tLift Off!" << std::endl;
			return MissionStageStatus::Completed;
		}
	} 
	return MissionStageStatus::InProgress;
}
