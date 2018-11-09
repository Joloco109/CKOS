#pragma once
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>
#include <memory>
#include "MissionInfo.h"

using namespace krpc;

struct MissionInfo {
	const std::shared_ptr<krpc::services::SpaceCenter> spacecenter;

	const std::shared_ptr<krpc::services::SpaceCenter::Vessel> vessel;

	std::shared_ptr<krpc::Stream<double>> ut;

	std::shared_ptr<krpc::Stream<double>> altitude;

	std::shared_ptr<krpc::Stream<float>> pitch;
	std::shared_ptr<krpc::Stream<float>> heading;
	std::shared_ptr<krpc::Stream<float>> roll;

	std::shared_ptr<krpc::Stream<std::tuple<double,double,double>>> orbit_prograde;
	std::shared_ptr<krpc::Stream<std::tuple<double,double,double>>> surface_prograde;

	MissionInfo(std::shared_ptr<krpc::services::SpaceCenter> _spacecenter, std::shared_ptr<krpc::services::SpaceCenter::Vessel> _vessel);
};
