#pragma once
#include "MissionStage.h"

using krpc::services::SpaceCenter;

class OrbitalInsertionStage : public MissionStage {
	double targetUt;
	double burnTime;

	bool startedBurn = false;

	std::unique_ptr<SpaceCenter::Node> m_node;
	
	krpc::Stream<std::tuple<double,double,double>> remaining;

	void createManeuver();

	public:
	OrbitalInsertionStage(const std::string& name, std::shared_ptr<MissionInfo> info);

	MissionStageStatus update();
};
