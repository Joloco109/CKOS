#pragma once
#include "MissionStage.h"
#include "Node.h"

using krpc::services::SpaceCenter;

class OrbitalInsertionStage : public MissionStage {
	double targetUt;
	double burnTime;

	bool startedBurn = false;

	krpc::Stream<std::tuple<double,double,double>> remaining;

	void createManeuver();

	std::unique_ptr<NodeStage> m_node;

	public:
	OrbitalInsertionStage(const std::string& name, std::shared_ptr<MissionInfo> info);

	std::shared_ptr<SpaceCenter::Node> getNode();

	MissionStageStatus update();
};
