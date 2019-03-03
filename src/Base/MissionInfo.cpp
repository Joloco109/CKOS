#include "MissionInfo.h"
#include "KMath.h"
#include <functional>

using krpc::services::SpaceCenter;
using krpc::Stream;

MissionInfo::MissionInfo(std::shared_ptr<krpc::Client> _connection, std::shared_ptr<SpaceCenter> _spacecenter, std::shared_ptr<SpaceCenter::Vessel> _vessel)
		: connection(_connection)
		, spacecenter(_spacecenter)
		, vessel(_vessel)
		, ut(std::make_shared<Stream<double>>(spacecenter->ut_stream()))
		, met(std::make_shared<Stream<double>>(vessel->met_stream()))
		, autopilot(std::make_shared<SpaceCenter::AutoPilot>(vessel->auto_pilot()))
		, control(std::make_shared<SpaceCenter::Control>(vessel->control()))
		, flight(std::make_shared<SpaceCenter::Flight>(vessel->flight()))
		, orbit(std::make_shared<SpaceCenter::Orbit>(vessel->orbit()))
		, altitude(std::make_shared<Stream<double>>(vessel->flight().surface_altitude_stream()))
		, pitch(std::make_shared<Stream<float>>(vessel->flight().pitch_stream()))
		, heading(std::make_shared<Stream<float>>(vessel->flight().heading_stream()))
		, roll(std::make_shared<Stream<float>>(vessel->flight().roll_stream()))
		, orbit_frame(vessel->orbit().body().non_rotating_reference_frame())
		, surface_frame(vessel->orbit().body().reference_frame())
		, orbit_prograde(std::make_shared<Stream<std::tuple<double,double,double>>>(
					vessel->flight(orbit_frame).prograde_stream()))
		, orbit_speed(std::make_shared<krpc::Stream<double>>(
					vessel->flight(orbit_frame).speed_stream()))
		, surface_speed(std::make_shared<krpc::Stream<double>>(
					vessel->flight(surface_frame).speed_stream()))
		, orbit_stream(vessel->orbit_stream())
	{
		orbit_stream.add_callback(
			std::bind(&MissionInfo::onOrbitChange, this, std::placeholders::_1));
	}

boost::numeric::ublas::bounded_vector<double,3> MissionInfo::surface_prograde() const {
	return k_math::to_uvector3(
			spacecenter->transform_direction(
			std::make_tuple(0,1,0),
			vessel->surface_velocity_reference_frame(),
			vessel->surface_reference_frame()
			));
}

void MissionInfo::onOrbitChange(SpaceCenter::Orbit newOrbit) {
	orbit = std::make_shared<SpaceCenter::Orbit>(newOrbit);
}
