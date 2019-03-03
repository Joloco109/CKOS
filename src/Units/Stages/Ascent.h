#pragma once
#include "MissionStage.h"
#include <limits.hpp>

namespace Stages {

class AscentStage : public MissionStage {
	double targetApoapsis;
	double maxAoA;
	double heading;

	double startTurnSpeed;
	double startTurnAngle;
	double midTurnSpeed;
	double midTurnAngle;
	double endTurnSpeed;
	double endTurnAngle;

	double midTurnAnglePerSpeed;
	double midTurnAngleSpeed_0;
	double endTurnAnglePerSpeed;
	double endTurnAngleSpeed_0;

	bool reachedGravityTurn = false;
	bool reachedMidGravityTurn = false;

	void pilot();

	public:
	AscentStage(const std::string& name, std::shared_ptr<MissionInfo> info, 
		double _targetApoapsis = 150000,
		double _maxAoA = 20,
		double _heading = std::numeric_limits<double>::quiet_NaN(),
		double _startTurnSpeed = 150,
		double _startTurnAngle = 2.5,
		double _midTurnSpeed = 700,
		double _midTurnAngle = 45,
		double _endTurnSpeed = 1750,
		double _endTurnAngle = 90
			);

	MissionStageStatus update() override;
};

}
