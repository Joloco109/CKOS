#pragma once
#include "MissionStage.h"

using krpc::services::SpaceCenter;

namespace Stages {

class NodeStage : public MissionStage {
	enum BurnPhase {
		scheduled, mainPhase, tuningPhase, completed
	};

	BurnPhase m_phase;

	double m_targetUt;
	double m_burnTime;

	std::shared_ptr<SpaceCenter::Node> m_node;
	
	const bool m_deleteNode;

	krpc::Stream<std::tuple<double,double,double>> m_remaining;

	public:
	NodeStage(const std::string& name, std::shared_ptr<MissionInfo> info, std::shared_ptr<SpaceCenter::Node> node, const bool deleteNode = true);

	MissionStageStatus update();
};

}
