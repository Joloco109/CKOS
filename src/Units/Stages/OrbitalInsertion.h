#pragma once
#include "MissionStage.h"
#include "Node.h"

using krpc::services::SpaceCenter;

namespace Stages {

class OrbitalInsertion : public MissionStage {
	double targetUt;
	double burnTime;

	bool startedBurn = false;

	krpc::Stream<std::tuple<double,double,double>> remaining;

	void createManeuver();

	std::unique_ptr<NodeStage> m_node;

	public:
	OrbitalInsertion(const std::string& name, std::shared_ptr<MissionInfo> info);

	std::shared_ptr<SpaceCenter::Node> getNode();

	MissionStageStatus update();
};

}
