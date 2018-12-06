#pragma once
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>
#include <memory>

using namespace krpc::services;

struct MissionInfo {
	const std::shared_ptr<SpaceCenter> spacecenter;

	const std::shared_ptr<SpaceCenter::Vessel> vessel;

	const std::shared_ptr<krpc::Stream<double>> ut;
	const std::shared_ptr<krpc::Stream<double>> met;

	const std::shared_ptr<krpc::Stream<double>> altitude;

	const std::shared_ptr<krpc::Stream<float>> pitch;
	const std::shared_ptr<krpc::Stream<float>> heading;
	const std::shared_ptr<krpc::Stream<float>> roll;

	//std::shared_ptr<krpc::Stream<SpaceCenter::ReferenceFrame>> orbit_frame;
	//std::shared_ptr<krpc::Stream<SpaceCenter::ReferenceFrame>> surface_frame;

	SpaceCenter::ReferenceFrame orbit_frame;
	SpaceCenter::ReferenceFrame surface_frame;


	std::shared_ptr<krpc::Stream<std::tuple<double,double,double>>> orbit_prograde;
	std::shared_ptr<krpc::Stream<std::tuple<double,double,double>>> surface_prograde;

	std::shared_ptr<krpc::Stream<double>> orbit_speed;
	std::shared_ptr<krpc::Stream<double>> surface_speed;

	MissionInfo(std::shared_ptr<krpc::services::SpaceCenter> _spacecenter, std::shared_ptr<krpc::services::SpaceCenter::Vessel> _vessel);
};
