#include "AscentStage.h"
#include "Math.h"

AscentStage::AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info, 
		double _targetApoapsis,
		double _maxAoA,
		double _heading,
		double _startTurnSpeed,
		double _startTurnAngle,
		double _midTurnSpeed,
		double _midTurnAngle,
		double _endTurnSpeed,
		double _endTurnAngle
			) 
	: MissionStage(name, info)
	, targetApoapsis(_targetApoapsis)
	, maxAoA(_maxAoA)
	, heading(_heading)
	, startTurnSpeed(_startTurnSpeed)
	, startTurnAngle(_startTurnAngle)
	, midTurnSpeed(_midTurnSpeed)
	, midTurnAngle(_midTurnAngle)
	, endTurnSpeed(_endTurnSpeed)
	, endTurnAngle(_endTurnAngle)
{ }


MissionStageStatus AscentStage::update() {
	if (activate()) {
		m_control.set_throttle(1);
		m_auto_pilot.engage();
		m_auto_pilot.set_reference_frame(m_vessel->surface_reference_frame());
		midTurnAnglePerSpeed = (midTurnAngle-startTurnAngle)/(midTurnSpeed-startTurnSpeed);
		midTurnAngleSpeed_0 = (midTurnSpeed*startTurnAngle-startTurnSpeed*midTurnAngle) / (midTurnSpeed-startTurnSpeed);
		endTurnAnglePerSpeed = (endTurnAngle-midTurnAngle)/(endTurnSpeed-midTurnSpeed);
		endTurnAngleSpeed_0 = (endTurnSpeed*midTurnAngle-midTurnSpeed*endTurnAngle) / (endTurnSpeed-midTurnSpeed);
	}

	int stage = m_control.current_stage();
	float liquid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("LiquidFuel");
	float solid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("SolidFuel");
	
	if (liquid_fuel < 0.1 && solid_fuel < 0.1) {
		m_control.activate_next_stage();
	}

	pilot();

	if (m_orbit.apoapsis_altitude() > targetApoapsis) {
		logger.info("Reached Apoapsis of {:.5}m", m_orbit.apoapsis_altitude());
		m_control.set_throttle(0);
		return MissionStageStatus::Completed;
	}
	return MissionStageStatus::InProgress;
}

void AscentStage::pilot() {
	boost_uvector2f pitch_heading;
	boost_uvector2f target_pitch_heading;
	if (surface_speed() < startTurnSpeed) {
		pitch_heading[0] = 90-startTurnAngle;
		if (heading == heading)
			pitch_heading[1] = heading;
		else
			pitch_heading[1] = 90;
	}
	else if (surface_speed() < midTurnSpeed) {
		if (!reachedGravityTurn) {
			reachedGravityTurn = true;
			logger.info("Reached Gravity Turn");
		}
		target_pitch_heading[0] = 90 - midTurnAnglePerSpeed*surface_speed() - midTurnAngleSpeed_0;
		target_pitch_heading[1] = 90;
		auto direction = m_info->spacecenter->transform_direction(
					std::make_tuple(0,1,0),
					m_vessel->surface_velocity_reference_frame(),
					m_vessel->surface_reference_frame()
				);
		pitch_heading = k_math::to_pitch_heading(k_math::to_uvector3(direction));
		pitch_heading[0] -= maxAoA * copysign(
				exp(-2*abs(pitch_heading[0]-target_pitch_heading[0])/maxAoA),
				pitch_heading[0]-target_pitch_heading[0]);
		if (heading == heading)
			pitch_heading[1] = heading;
	}
	else {
		if (!reachedMidGravityTurn) {
			reachedMidGravityTurn = true;
			logger.info("Reached Mid-Gravity Turn");
		}
		auto direction = m_info->spacecenter->transform_direction(
					std::make_tuple(0,1,0),
					m_vessel->surface_velocity_reference_frame(),
					m_vessel->surface_reference_frame()
				);
		pitch_heading = k_math::to_pitch_heading(k_math::to_uvector3(direction));
	}
	if (m_orbit.apoapsis_altitude() > targetApoapsis)
		m_control.set_throttle(0.02);
	set_pitch_heading(pitch_heading[0], pitch_heading[1]);
}
