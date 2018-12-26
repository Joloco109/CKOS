#include <cmath>

#include "SuicideBurn.h"
#include "../Math.h"

namespace Stages {

SuicideBurn::SuicideBurn(const std::string& name, std::shared_ptr<MissionInfo> info) 
	: MissionStage(name, info)
	, m_velocity(std::make_shared<krpc::Stream<vector3>>(m_vessel->flight(m_orbit->body().reference_frame()).velocity_stream()))
{ }


MissionStageStatus SuicideBurn::update() {
	bool first = false;
	if (activate()) {
		m_auto_pilot->disengage();
		m_control->set_sas(true);
		m_control->set_sas_mode(SpaceCenter::SASMode::retrograde);
		first = true;
	}

	int stage = m_control->current_stage();
	float liquid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("LiquidFuel");
	float solid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("SolidFuel");
	
	if (liquid_fuel < 0.1 && solid_fuel < 0.1) {
		m_control->activate_next_stage();
	}

	pilot();

	if (first) {
		//double time = 

		logger.info("Starting at Altitude: {}", altToBurn);
	}

	if (m_vessel->flight().surface_altitude() < 0.2) {
		m_control->set_throttle(0);
		m_control->set_sas(true);
		m_control->set_sas_mode(SpaceCenter::SASMode::stability_assist);
		logger.info("Landing Comlplete");
		return MissionStageStatus::Completed;
	}
	return MissionStageStatus::InProgress;
}

void SuicideBurn::pilot() {
	float Force = m_vessel->available_thrust();
	float mass = m_vessel->mass();
	float acc = Force/mass;
	float g = m_orbit->body().surface_gravity();
	vector3 v0 = m_info->spacecenter->transform_velocity(m_vessel->flight(m_orbit->body().reference_frame()).center_of_mass(), (*m_velocity)(), m_orbit->body().reference_frame(), m_vessel->surface_reference_frame());
	double vx = sqrt(std::get<1>(v0)*std::get<1>(v0)+std::get<2>(v0)*std::get<2>(v0));
	double vy = std::get<0>(v0);
	altToBurn = -(
			 pow(vy,2) / (acc-g)
			-pow(vx,2) * (1-g/acc)/acc
			 )/2;

	if (altToBurn*1.2 < m_vessel->flight().surface_altitude() && sqrt(pow(vx,2) + pow(vy,2)) > 1)
		m_control->set_throttle(1);
	else
		m_control->set_throttle(0);
}

}
