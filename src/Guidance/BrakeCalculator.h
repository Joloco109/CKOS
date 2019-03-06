#pragma once

#include <krpc.hpp>
#include <krpc/services/space_center.hpp>

#include <boost/numeric/ublas/vector.hpp>

typedef boost::numeric::ublas::bounded_vector<double, 4> odeint_2dstate;
typedef boost::numeric::ublas::bounded_vector<double, 6> odeint_3dstate;

class Breaker {
	public:
	class BreakerSystem;
	class BreakerObserver;

	friend BreakerSystem;

	private:
	std::shared_ptr<krpc::services::SpaceCenter> m_spacecenter;
	std::shared_ptr<krpc::services::SpaceCenter::Vessel> m_vessel;

	double m_F;
	double m_m0;
	double m_g;

	const std::shared_ptr<BreakerSystem> m_breaker_system;
	const std::shared_ptr<BreakerObserver> m_breaker_observer;

	public:
	Breaker(
			std::shared_ptr<krpc::services::SpaceCenter> spacecenter,
			std::shared_ptr<krpc::services::SpaceCenter::Vessel> vessel,
			double F, double m0, double g);

	void set_F(double F);
	void set_m0(double m0);
	void set_g(double g);

	odeint_2dstate standardTrajectory(
			krpc::services::SpaceCenter::Orbit orbit,
		       	krpc::services::SpaceCenter::CelestialBody body,
			double t);

	std::tuple<odeint_2dstate, double> burnStartLocTime(
			double t0,
			odeint_2dstate (*trajectory)(double));

	std::tuple<odeint_2dstate, double> burnStartLocTime(
			double t0,
			std::function<odeint_2dstate(double)> trajectory);

	std::tuple<odeint_2dstate, double> burnStartLocTime(
			double t0,
			krpc::services::SpaceCenter::Orbit orbit,
			krpc::services::SpaceCenter::CelestialBody body);
};
