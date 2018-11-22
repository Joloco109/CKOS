#pragma once
#include "MissionStage.h"

class AscentStage : public MissionStage {
	double startTurnSpeed = 100;
	double startTurnAlt = 100;
	double startTurnAngle = 2.5;
	double midTurnSpeed = 1000;
	double midTurnAlt = 30000;
	double midTurnAngle = 45;
	double endTurnSpeed = 2500;
	double endTurnAlt = 35000;
	double endTurnAngle = 90;
	double targetApoapsis = 150000;
	double maxAoA = 5;

	void pilot();

	public:
	AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info);

	MissionStageStatus update();
};
