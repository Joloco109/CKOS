#include "MissionInfo.h"

using krpc::services::SpaceCenter;
using krpc::Stream;

MissionInfo::MissionInfo(std::shared_ptr<SpaceCenter> _spacecenter, std::shared_ptr<SpaceCenter::Vessel> _vessel)
		: spacecenter(_spacecenter)
		, vessel(_vessel)
		, ut(std::make_shared<Stream<double>>(spacecenter->ut_stream()))
		, met(std::make_shared<Stream<double>>(vessel->met_stream()))
		, altitude(std::make_shared<Stream<double>>(vessel->flight().surface_altitude_stream()))
		, pitch(std::make_shared<Stream<float>>(vessel->flight().pitch_stream()))
		, heading(std::make_shared<Stream<float>>(vessel->flight().heading_stream()))
		, roll(std::make_shared<Stream<float>>(vessel->flight().roll_stream()))
		, orbit_frame(vessel->orbit().body().non_rotating_reference_frame())
		, surface_frame(vessel->orbit().body().reference_frame())
		/*, orbit_frame(std::make_shared<Stream<SpaceCenter::ReferenceFrame>>(
					vessel->orbit().body().non_rotating_reference_frame()))
		, surface_frame(std::make_shared<Stream<SpaceCenter::ReferenceFrame>>(
					vessel->orbit().body().reference_frame()))*/
		, orbit_prograde(std::make_shared<Stream<std::tuple<double,double,double>>>(
					vessel->flight(orbit_frame).prograde_stream()))
		, surface_prograde(std::make_shared<Stream<std::tuple<double,double,double>>>(
					vessel->flight(surface_frame).prograde_stream()))
		, orbit_speed(std::make_shared<krpc::Stream<double>>(
					vessel->flight(orbit_frame).speed_stream()))
		, surface_speed(std::make_shared<krpc::Stream<double>>(
					vessel->flight(surface_frame).speed_stream()))
	{
		
	}
