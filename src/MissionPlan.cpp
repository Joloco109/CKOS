#include "MissionPlan.h"

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
	logger.log(spdlog::level::info, "MissionPlaner created");
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
			m_current = m_current->m_next;
			logger.log(spdlog::level::info, "Stage {} reached.", m_current->getName());
		}
		else {
			logger.log(spdlog::level::info, "Mission complete!");
			return false;
		}
	else if (status == MissionStageStatus::Failed) {
		logger.log(spdlog::level::info, "Mission failure!");
		return false;
	}
	return true;
}

std::shared_ptr<MissionStage> MissionPlaner::current() const {
	return m_current;
}
