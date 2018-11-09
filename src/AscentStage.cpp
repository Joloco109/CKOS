#include "AscentStage.h"

AscentStage::AscentStage(std::string name, std::shared_ptr<MissionInfo> info) : MissionStage(name, info) {}

MissionStageStatus AscentStage::update() {
	if (activate())
		m_info->vessel->control().activate_next_stage();
	return MissionStageStatus::InProgress;
}
