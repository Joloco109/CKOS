#pragma once
#include "MissionStage.h"
#include <limits.hpp>

class AscentStage : public MissionStage {
	double targetApoapsis = 150000;
	double maxAoA = 5;
	double heading;

	double startTurnSpeed = 100;
	double startTurnAngle = 2.5;
	double midTurnSpeed = 550;
	double midTurnAngle = 45;
	double endTurnSpeed = 1500;
	double endTurnAngle = 90;

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
		double _maxAoA = 5,
		double _heading = std::numeric_limits<double>::quiet_NaN(),
		double _startTurnSpeed = 100,
		double _startTurnAngle = 2.5,
		double _midTurnSpeed = 700,
		double _midTurnAngle = 45,
		double _endTurnSpeed = 1500,
		double _endTurnAngle = 90
			);

	MissionStageStatus update();
};
