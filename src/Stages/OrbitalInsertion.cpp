#include "OrbitalInsertion.h"
#include "../Math.h"

OrbitalInsertionStage::OrbitalInsertionStage(const std::string& name, std::shared_ptr<MissionInfo> info)
: MissionStage(name, info)
{ }

void OrbitalInsertionStage::createManeuver() {
	double GM = m_orbit->body().gravitational_parameter();
	double r = m_orbit->apoapsis();
	double a1 = m_orbit->semi_major_axis();
	double v1 = std::sqrt(GM*(2/r-1/a1));
	double v2 = std::sqrt(GM*(1/r));
	double delta_v = v2 -v1;
	m_node = std::unique_ptr<SpaceCenter::Node>(
			new SpaceCenter::Node(m_control->add_node(
					m_info->ut->operator()() + m_orbit->time_to_apoapsis(), 
					delta_v
			)));
	double F = m_vessel->available_thrust();
	double Isp = m_vessel->specific_impulse() * 9.82;
	double m0 = m_vessel->mass();
	double m1 = m0 / std::exp(delta_v/Isp);
	double flow_rate = F/Isp;
	burnTime = (m0-m1)/flow_rate;
	targetUt = m_node->ut() - burnTime/2;
	remaining = m_node->remaining_burn_vector_stream(m_node->reference_frame());
}

MissionStageStatus OrbitalInsertionStage::update() {
	if (activate()) {
		createManeuver();
		m_auto_pilot->set_reference_frame(m_node->reference_frame());
		m_auto_pilot->set_target_direction(std::make_tuple(0,1,0));
		logger.info("Orienting Ship for Orbital Insertion Burn");
	}
	if (m_auto_pilot->error() < boost::numeric::ublas::norm_2(k_math::to_uvector3(m_auto_pilot->attenuation_angle()))) {
		if (startedBurn && std::get<1>(remaining()) < 0) {
			m_control->set_throttle(0);
			logger.info("Completed Orbital Insertion");
			return MissionStageStatus::Completed;
		}
		else if (startedBurn&& m_info->ut->operator()() > targetUt + burnTime - 0.3) {
			m_control->set_throttle(0.05);
			logger.info("Performing fine tuning");

		}
		else if (m_info->ut->operator()() > targetUt) {
			startedBurn = true;
			m_control->set_throttle(1);
			logger.info("Executing burn");
		}
	}
	return MissionStageStatus::InProgress;
}
