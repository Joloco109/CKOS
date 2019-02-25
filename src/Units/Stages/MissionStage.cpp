#include "MissionStage.h"

namespace Stages {

/*
 * Definitions of MissionStage
 *
 */

MissionStage::MissionStage(const std::string& name, std::shared_ptr<MissionInfo> info):
	Unit(name, info),
	m_info(info),
	m_name(name) 
{
	logger.trace("MissionStage {} created", m_name);
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
		logger.info("Activated Stage: {}", m_name);
		return true;
	}
	return false;
}

/*
 * Definitions of CountdownStage
 *
 */

CountdownStage::CountdownStage(const std::string& name, std::shared_ptr<MissionInfo> info) :
	MissionStage(name, info)
{}

CountdownStage::CountdownStage(const std::string& name, std::shared_ptr<MissionInfo> info, double seconds) :
	MissionStage(name, info),
	m_seconds(seconds) 
{}

MissionStageStatus CountdownStage::update() {
	if (activate()) {
		m_launchtime = m_seconds + getInfo()->ut->operator()();
		logger.info("Launch at {} ", m_launchtime);
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

}


/*
 * Definition of MissionPlaner members
 *
 */

MissionPlaner::MissionPlaner(std::shared_ptr<MissionInfo> info) :
	Unit("MissionPlaner", info),
	m_first(std::make_shared<CountdownStage>("Countdown", info)),
	m_current(m_first),
	m_info(info)
{
	logger.info("MissionPlaner created");
}

bool MissionPlaner::addStage(std::shared_ptr<MissionStage> stage) {
	std::shared_ptr<MissionStage> current = m_first;
	while (current->m_next)
		current = current->m_next;
	current->m_next = stage;
	return true;
}

bool MissionPlaner::update() {
	if (!m_current)
		return false;
	MissionStageStatus status = m_current->update();
	if (status == MissionStageStatus::Completed)
		if (m_current->m_next != NULL) {
			m_current = m_current->m_next;
			logger.info("Stage {} reached.", m_current->getName());
		}
		else {
			logger.info("Mission complete!");
			return false;
		}
	else if (status == MissionStageStatus::Failed) {
		logger.info("Mission failure!");
		return false;
	}
	return true;
}

std::shared_ptr<MissionStage> MissionPlaner::current() const {
	return m_current;
}
