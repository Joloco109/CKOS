#include "AscentStage.h"

AscentStage::AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info) : MissionStage(name, info) {}

MissionStageStatus AscentStage::update() {
	if (activate()) {
		m_info->vessel->control().set_throttle(1);
		m_info->vessel->auto_pilot().engage();
	}
	if (m_info->vessel->resources_in_decouple_stage(m_info->vessel->control().current_stage()).amount("LiquidFuel") < 0.1)
		m_info->vessel->control().activate_next_stage();
	return MissionStageStatus::InProgress;
}
