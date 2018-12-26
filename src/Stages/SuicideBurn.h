#pragma once
#include "MissionStage.h"
#include "../Math.h"

namespace Stages {

class SuicideBurn : public MissionStage {

	double altToBurn;
	std::shared_ptr<krpc::Stream<vector3>> m_velocity;

	void pilot();

	public:
	SuicideBurn(const std::string& name, std::shared_ptr<MissionInfo> info);
	MissionStageStatus update() override;
};

}
