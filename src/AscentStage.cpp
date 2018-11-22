#include "AscentStage.h"

AscentStage::AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info) : MissionStage(name, info) {}

MissionStageStatus AscentStage::update() {
	if (activate()) {
		m_info->vessel->control().set_throttle(1);
		m_info->vessel->auto_pilot().engage();
	}

	int stage = m_info->vessel->control().current_stage();
	float liquid_fuel = m_info->vessel->resources_in_decouple_stage(stage-1).amount("LiquidFuel");
	float solid_fuel = m_info->vessel->resources_in_decouple_stage(stage-1).amount("SolidFuel");
	
	if (liquid_fuel < 0.1 && solid_fuel < 0.1) {
		m_info->vessel->control().activate_next_stage();
	}

	pilot();

	if (m_info->vessel->orbit().apoapsis_altitude() > targetApoapsis)
		return MissionStageStatus::Completed;
	return MissionStageStatus::InProgress;
}

void AscentStage::pilot() {
}
