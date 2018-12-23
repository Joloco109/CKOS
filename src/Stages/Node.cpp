#include "Node.h"
#include "../Math.h"

NodeStage::NodeStage(const std::string& name, std::shared_ptr<MissionInfo> info, std::shared_ptr<SpaceCenter::Node> node, const bool deleteNode)
	: MissionStage(name, info)
	, m_node(node)
	, m_deleteNode(deleteNode)
{
	double delta_v = m_node->delta_v();
	double F = m_vessel->available_thrust();
	double Isp = m_vessel->specific_impulse() * 9.82;
	double m0 = m_vessel->mass();
	double m1 = m0 / std::exp(delta_v/Isp);
	double flow_rate = F/Isp;
	m_burnTime = (m0-m1)/flow_rate;
	m_targetUt = m_node->ut() - m_burnTime/2;
	m_remaining = m_node->remaining_burn_vector_stream(m_vessel->reference_frame());
}

MissionStageStatus NodeStage::update() {
	if (activate()) {
		m_auto_pilot->set_reference_frame(m_node->reference_frame());
		m_auto_pilot->set_target_direction(std::make_tuple(0,1,0));
		logger.info("Orienting Ship for Orbital Insertion Burn");
	}
	//if (m_auto_pilot->error() < boost::numeric::ublas::norm_2(k_math::to_uvector3(m_auto_pilot->attenuation_angle()))) {
	if (m_phase == tuningPhase && std::get<1>(m_remaining()) < 0) {
		m_control->set_throttle(0);
		m_phase = completed;
		logger.info("Completed Burn");
		if (m_deleteNode) {
			m_node->remove();
		}
		return MissionStageStatus::Completed;
	}
	else if (m_phase == mainPhase && m_info->ut->operator()() > m_targetUt + m_burnTime - 0.3) {
		m_control->set_throttle(0.1);
		m_phase = tuningPhase;
		logger.info("Performing fine tuning");

	}
	else if (m_phase == scheduled &&m_info->ut->operator()() > m_targetUt) {
		m_control->set_throttle(1);
		m_phase = mainPhase;
		logger.info("Executing burn");
	}
	//}
	return MissionStageStatus::InProgress;
}
