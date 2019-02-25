#include "OrbitalInsertion.h"

namespace Stages {

OrbitalInsertion::OrbitalInsertion(const std::string& name, std::shared_ptr<MissionInfo> info)
: MissionStage(name, info)
{ }

void OrbitalInsertion::createManeuver() {
	double GM = m_orbit->body().gravitational_parameter();
	double r = m_orbit->apoapsis();
	double a1 = m_orbit->semi_major_axis();
	double v1 = std::sqrt(GM*(2/r-1/a1));
	double v2 = std::sqrt(GM*(1/r));
	double delta_v = v2 -v1;
	m_node = std::unique_ptr<NodeStage>( new NodeStage( m_name + " Node", m_info,
		std::shared_ptr<SpaceCenter::Node>(
			new SpaceCenter::Node(m_control->add_node(
					m_info->ut->operator()() + m_orbit->time_to_apoapsis(), 
					delta_v
			)))));
}

MissionStageStatus OrbitalInsertion::update() {
	if (activate()) {
		createManeuver();
	}
	if (MissionStageStatus::Completed == m_node->update()) {
		logger.info("Oribtal Insertion Completed with Apoapsis {:.5}m, Perisapses {:.5}m", m_orbit->apoapsis_altitude(), m_orbit->periapsis_altitude());
		return MissionStageStatus::Completed;
	}
	return MissionStageStatus::InProgress;
}

}
