#pragma once
#include "MissionStage.h"

class AscentStage : public MissionStage {
	double turnStart = 100;
	double endTurn = 35000;
	double startAngle = 2.5;
	double targetApoapsis = 150000;
	double maxAoA = 5;

	public:
	AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info);

	MissionStageStatus update();
};
