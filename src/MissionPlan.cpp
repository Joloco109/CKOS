#include "MissionPlan.h"

/*
 * Definitions of MissionStage
 *
 */

MissionStage::MissionStage(std::string name, std::shared_ptr<MissionInfo> info): m_info(info),m_name(name) {
	std::cout << "MissionStage " << m_name << " created!" << std::endl;
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
		std::cout << "Activated Stage: " << m_name << std::endl;
		return true;
	}
	return false;
}

/*
 * Definitions of CountdownStage
 *
 */

CountdownStage::CountdownStage(std::string name, std::shared_ptr<MissionInfo> info): MissionStage(name, info) {}

CountdownStage::CountdownStage(std::string name, std::shared_ptr<MissionInfo> info, double seconds): MissionStage(name, info), m_seconds(seconds) {}

MissionStageStatus CountdownStage::update() {
	if (activate()) {
		m_launchtime = m_seconds + getInfo()->ut->operator()();
		std::cout << "Launch at " << m_launchtime << std::endl;
	}
	double time = getInfo()->ut->operator()();
	if (m_launchtime - m_seconds < time) {
		if (m_seconds > 0) {
			std::cout << "T-" << m_seconds << std::endl;
			m_seconds--;
		}
		else {
			std::cout << "Lift Off!" << std::endl;
			return MissionStageStatus::Completed;
		}
	} 
	return MissionStageStatus::InProgress;
}

/*
 * Definition of MissionPlaner members
 *
 */

MissionPlaner::MissionPlaner(std::shared_ptr<MissionInfo> info) {
	m_first = std::make_shared<CountdownStage>("Countdown", info);
	m_current = m_first;
	m_info = info;
	std::cout << "MissionPlaner Created!" << std::endl;
}

bool MissionPlaner::addStage(std::shared_ptr<MissionStage> stage) {
	std::shared_ptr<MissionStage> current = m_first;
	while (current->m_next)
		current = current->m_next;
	current->m_next = stage;
	return true;
}

bool MissionPlaner::update() {
	MissionStageStatus status = m_current->update();
	if (status == MissionStageStatus::Completed)
		if (m_current->m_next != NULL) {
			std::cout << "Stage " << m_current->getName() << " reached." << std::endl;
			m_current = m_current->m_next;
		}
		else {
			std::cout << "Mission complete!" << std::endl;
			return false;
		}
	else if (status == MissionStageStatus::Failed) {
		std::cout << "Mission Failure" << std::endl;
		return false;
	}
	return true;
}

std::shared_ptr<MissionStage> MissionPlaner::current() const {
	return m_current;
}
