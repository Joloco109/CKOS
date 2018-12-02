#include "AscentStage.h"
#include "Math.h"

AscentStage::AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info) : MissionStage(name, info) {}

MissionStageStatus AscentStage::update() {
	if (activate()) {
		m_info->vessel->control().set_throttle(1);
		m_info->vessel->auto_pilot().engage();
		m_info->vessel->auto_pilot().set_reference_frame(m_info->vessel->surface_reference_frame());
		midTurnAnglePerSpeed = (midTurnAngle-startTurnAngle)/(midTurnSpeed-startTurnSpeed);
		midTurnAngleSpeed_0 = (midTurnSpeed*startTurnAngle-startTurnSpeed*midTurnAngle) / (midTurnSpeed-startTurnSpeed);
		endTurnAnglePerSpeed = (endTurnAngle-midTurnAngle)/(endTurnSpeed-midTurnSpeed);
		endTurnAngleSpeed_0 = (endTurnSpeed*midTurnAngle-midTurnSpeed*endTurnAngle) / (endTurnSpeed-midTurnSpeed);
	}

	int stage = m_info->vessel->control().current_stage();
	float liquid_fuel = m_info->vessel->resources_in_decouple_stage(stage-1).amount("LiquidFuel");
	float solid_fuel = m_info->vessel->resources_in_decouple_stage(stage-1).amount("SolidFuel");
	
	if (liquid_fuel < 0.1 && solid_fuel < 0.1) {
		m_info->vessel->control().activate_next_stage();
	}

	pilot();

	if (m_info->vessel->orbit().apoapsis_altitude() > targetApoapsis) {
		logger.info("Reached Apoapsis of {:.5}m", m_info->vessel->orbit().apoapsis_altitude());
		m_info->vessel->control().set_throttle(0);
		return MissionStageStatus::Completed;
	}
	return MissionStageStatus::InProgress;
}

void AscentStage::pilot() {
	boost::numeric::ublas::bounded_vector<float,2> pitch_heading;
	if (m_info->surface_speed->operator()() < startTurnSpeed) {
		pitch_heading[0] = 90-startTurnAngle;
		pitch_heading[1] = 90;
	}
	else if (m_info->surface_speed->operator()() < midTurnSpeed) {
		if (!reachedGravityTurn) {
			reachedGravityTurn = true;
			logger.info("Reached Gravity Turn");
		}
		pitch_heading[0] = 90 - midTurnAnglePerSpeed*m_info->surface_speed->operator()() - midTurnAngleSpeed_0;
		pitch_heading[1] = 90;
		/*auto direction = m_info->spacecenter->transform_direction(
					std::make_tuple(0,1,0),
					m_info->vessel->surface_velocity_reference_frame(),
					m_info->vessel->surface_reference_frame()
				);
		pitch_heading = k_math::to_pitch_heading(k_math::to_uvector3(direction));*/
	}
	else {
		if (!reachedMidGravityTurn) {
			reachedMidGravityTurn = true;
			logger.info("Reached Mid-Gravity Turn");
		}
		pitch_heading[0] = 90 - endTurnAnglePerSpeed*m_info->surface_speed->operator()() - endTurnAngleSpeed_0;
		pitch_heading[1] = 90;
		auto direction = m_info->spacecenter->transform_direction(
					std::make_tuple(0,1,0),
					m_info->vessel->surface_velocity_reference_frame(),
					m_info->vessel->surface_reference_frame()
				);
		pitch_heading = k_math::to_pitch_heading(k_math::to_uvector3(direction));
	}
	if (m_info->vessel->orbit().apoapsis_altitude() > targetApoapsis)
		m_info->vessel->control().set_throttle(0.1);
	if (pitch_heading[0] < 0)
		pitch_heading[0] = 0;
	m_info->vessel->auto_pilot().target_pitch_and_heading(pitch_heading[0], pitch_heading[1]);
}
