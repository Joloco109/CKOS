#pragma once
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>
#include <memory>
#include <boost/numeric/ublas/vector.hpp>

using namespace krpc::services;

struct MissionInfo {
	//Member Variables
	
	const std::shared_ptr<SpaceCenter> spacecenter;

	const std::shared_ptr<SpaceCenter::Vessel> vessel;

	const std::shared_ptr<krpc::Stream<double>> ut;
	const std::shared_ptr<krpc::Stream<double>> met;

	std::shared_ptr<SpaceCenter::AutoPilot> autopilot;
	std::shared_ptr<SpaceCenter::Control> control;
	std::shared_ptr<SpaceCenter::Flight> flight;

	std::shared_ptr<SpaceCenter::Orbit> orbit;

	const std::shared_ptr<krpc::Stream<double>> altitude;

	const std::shared_ptr<krpc::Stream<float>> pitch;
	const std::shared_ptr<krpc::Stream<float>> heading;
	const std::shared_ptr<krpc::Stream<float>> roll;

	SpaceCenter::ReferenceFrame orbit_frame;
	SpaceCenter::ReferenceFrame surface_frame;

	std::shared_ptr<krpc::Stream<std::tuple<double,double,double>>> orbit_prograde;

	std::shared_ptr<krpc::Stream<double>> orbit_speed;
	std::shared_ptr<krpc::Stream<double>> surface_speed;


	//Member Funtions

	MissionInfo(std::shared_ptr<krpc::services::SpaceCenter> _spacecenter, std::shared_ptr<krpc::services::SpaceCenter::Vessel> _vessel);

	boost::numeric::ublas::bounded_vector<double, 3> surface_prograde() const;

	void onOrbitChange(SpaceCenter::Orbit newOrbit);

	private:
       	krpc::Stream<SpaceCenter::Orbit> orbit_stream;
};
