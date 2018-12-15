#pragma once
/*#include <krpc.hpp>
#include <krpc/services/krpc.hpp>*/

#include <memory>

#include "Stages/MissionStage.h"
#include "CKOS_Unit.h"

class MissionPlaner : Unit {
	std::shared_ptr<MissionStage> m_first;
	std::shared_ptr<MissionStage> m_current;
	std::shared_ptr<MissionInfo> m_info;

	public:
	MissionPlaner(std::shared_ptr<MissionInfo> info);

	bool addStage(std::shared_ptr<MissionStage> stage);

	bool update();

	std::shared_ptr<MissionStage> current() const;
};
