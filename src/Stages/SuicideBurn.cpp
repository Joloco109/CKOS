#include <cmath>

#include "SuicideBurn.h"
#include "../Math.h"

namespace Stages {

SuicideBurn::SuicideBurn(const std::string& name, std::shared_ptr<MissionInfo> info) 
	: MissionStage(name, info)
	, m_velocity(std::make_shared<krpc::Stream<vector3>>(m_vessel->flight(m_orbit->body().reference_frame()).velocity_stream()))
	, m_breaker(m_info->spacecenter,m_info->vessel, 0,0,0)
{ }


MissionStageStatus SuicideBurn::update() {
	if (activate()) {
		m_auto_pilot->disengage();
		m_control->set_sas(true);
		m_control->set_sas_mode(SpaceCenter::SASMode::retrograde);

		pilot();
		
	}
	else {
		pilot();
	}

	int stage = m_control->current_stage();
	float liquid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("LiquidFuel");
	float solid_fuel = m_vessel->resources_in_decouple_stage(stage-1).amount("SolidFuel");
	
	if (liquid_fuel < 0.1 && solid_fuel < 0.1) {
		m_control->activate_next_stage();
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
	
	boost_uvector3 ey = k_math::to_uvector3(m_info->spacecenter->transform_direction(
				std::make_tuple<double, double, double>(1,0,0),
				m_vessel->surface_reference_frame(),
				m_orbit->body().reference_frame()));
	
	boost_uvector3 v0 = k_math::to_uvector3((*m_velocity)());
	double vy = boost::numeric::ublas::inner_prod(v0,ey);
	boost_uvector3 vh = v0 - vy*ey;
	double vx = boost::numeric::ublas::norm_2(vh);
	vy = abs(vy);

	altToBurn = vy*( vx/acc + vy/(acc-g));

	if (altToBurn > m_vessel->flight().surface_altitude() && sqrt(pow(vx,2) + pow(vy,2)) > 1)
		m_control->set_throttle(1);
	else
		m_control->set_throttle(0);

	m_breaker.set_F(Force);
	m_breaker.set_m0(mass);
	m_breaker.set_g(g);

	auto startLocTime = m_breaker.burnStartLocTime((*m_info->ut)(), m_vessel->orbit(), m_vessel->orbit().body());
	odeint_2dstate startX = std::get<0>(startLocTime);
	double startT = std::get<1>(startLocTime);
	std::cout << "Time to burn " << (startT - (*m_info->ut)()) << "" << std::endl;
	std::cout << "Landing location (" << startX[0] << "," << startX[1] << ")" << std::endl;
	std::cout << "Landing speed (" << startX[2] << "," << startX[3] << ")" << std::endl;
}

}
