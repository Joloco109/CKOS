#pragma once
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <memory>

#include "../MissionInfo.h"
#include "../CKOS_Unit.h"

#define m_vessel m_info->vessel
#define m_orbit m_info->orbit
#define m_control m_info->control
#define m_auto_pilot m_info->autopilot
#define set_pitch_heading m_info->autopilot->target_pitch_and_heading
#define surface_speed m_info->surface_speed->operator()

namespace Stages {

enum class MissionStageStatus {
	Scheduled,
	InProgress,
	Completed,
	Failed
};

class MissionStage : public Unit {
	
	protected:
	MissionStageStatus m_status = MissionStageStatus::Scheduled;
	std::shared_ptr<MissionInfo> m_info;
	const std::string m_name;

	public:
	MissionStage(const std::string& name, std::shared_ptr<MissionInfo> info);

	std::shared_ptr<MissionStage> m_next;

	MissionStageStatus getStatus() const;
	std::shared_ptr<MissionInfo> getInfo() const;
	std::string getName() const;

	virtual MissionStageStatus update() = 0;

	bool activate();
};

class CountdownStage : public MissionStage {
	double m_seconds = 10;
	double m_launchtime;

	public:
	CountdownStage(const std::string& name, std::shared_ptr<MissionInfo> info);
	CountdownStage(const std::string& name, std::shared_ptr<MissionInfo> info, double seconds);

	virtual MissionStageStatus update();
};

}
