#include "BrakeCalculator.h"
#include "../Math.h"
#include <cmath>
#include <boost/numeric/odeint.hpp>

Breaker::Breaker(
		std::shared_ptr<krpc::services::SpaceCenter> spacecenter,
		std::shared_ptr<krpc::services::SpaceCenter::Vessel> vessel,
		double F, double m0, double g)
	: m_spacecenter(spacecenter), m_vessel(vessel), m_F(F), m_m0(m0), m_g(g)
	, m_breaker_system(std::make_shared<BreakerSystem>(*this))
	, m_breaker_observer(std::make_shared<BreakerObserver>())
{ }

void Breaker::set_F(double F) {
	m_F = F;
}

void Breaker::set_m0(double m0) {
	m_m0 = m0;
}

void Breaker::set_g(double g) {
	m_g = g;
}

class Breaker::BreakerSystem {
	private:
	const Breaker& m_breaker;

	public:
	BreakerSystem(const Breaker& breaker);

	BreakerSystem(const BreakerSystem& breakersystem);

	void operator() (const odeint_2dstate &x, odeint_2dstate &dxdt, const double t) const;
};

Breaker::BreakerSystem::BreakerSystem(const Breaker& breaker) : m_breaker(breaker) {}

Breaker::BreakerSystem::BreakerSystem(const BreakerSystem& breakersystem) : m_breaker(breakersystem.m_breaker) {

}

void Breaker::BreakerSystem::operator() (const odeint_2dstate &x, odeint_2dstate &dxdt, const double t) const {
	dxdt[0] = x[2];
	dxdt[1] = x[3];
	dxdt[2] = - m_breaker.m_F/m_breaker.m_m0 * x[0]/sqrt(x[0]*x[0] + x[1]*x[1]);
	dxdt[3] = m_breaker.m_F/m_breaker.m_m0 * x[1]/sqrt(x[0]*x[0] + x[1]*x[1]) - m_breaker.m_g;
}

class Breaker::BreakerObserver {
	double m_T;
	odeint_2dstate m_xT;

	public:
	BreakerObserver();

	void operator() (const odeint_2dstate &x, const double &t);

	void reset() {
		m_T = 0;
		m_xT = static_cast<odeint_2dstate>(boost::numeric::ublas::zero_vector<double>(4));
	}

	const double& T() const {
		return m_T;
	}

	const odeint_2dstate& xT() const {
		return m_xT;
	}
};

Breaker::BreakerObserver::BreakerObserver() : m_T(0), m_xT(static_cast<odeint_2dstate>(boost::numeric::ublas::zero_vector<double>(4))) { }

void Breaker::BreakerObserver::operator() (const odeint_2dstate& x, const double& t) {
	if (abs(x[1]) < 1.0e-2) {
		m_T = t;
		m_xT = x;
	}
}

odeint_2dstate Breaker::standardTrajectory(
		krpc::services::SpaceCenter::Orbit orbit,
		krpc::services::SpaceCenter::CelestialBody body,
		double t)
{
	odeint_2dstate out;

	float dt = 0.01;
	auto ey = boost::numeric::ublas::unit_vector<double>(3,0);
	
	auto frame = m_vessel->surface_reference_frame();
	auto pos = k_math::to_uvector3(orbit.position_at(t,frame)) + body.altitude_at_position(orbit.position_at(t, frame), frame) * ey;
	auto pos_dt = k_math::to_uvector3(orbit.position_at(t+dt,frame)) + body.altitude_at_position(orbit.position_at(t+dt, frame), frame) * ey;

	auto vel = (pos_dt-pos)/dt;
	
	auto posy = boost::numeric::ublas::inner_prod(pos, ey)*ey;
	auto posx = pos - posy;
	
	auto vely = boost::numeric::ublas::inner_prod(vel,ey)*ey;
	auto velx = vel - vely;
	
	out[0] = boost::numeric::ublas::norm_2(posx);
	out[1] = boost::numeric::ublas::norm_2(posy);
	out[2] = boost::numeric::ublas::norm_2(velx);
	out[3] = boost::numeric::ublas::inner_prod(vely, ey);
	
	return out;
}

std::tuple<odeint_2dstate, double>Breaker::burnStartLocTime(
		double t0,
		std::function<odeint_2dstate(double)> trajectory)
{
	auto state0 = trajectory(t0);
	double T = abs(state0[0]*m_m0/m_F) + abs(state0[1]/(m_F/m_m0-m_g));
	double t = t0;
	double dt = T/2;
	int n = 0;
	do  {
		using namespace boost::numeric::odeint;
		auto stepper = make_dense_output(1.0e-3, 1.0e-3, runge_kutta_dopri5<odeint_2dstate>());
		m_breaker_observer->reset();
		auto x = trajectory(t+dt);
		double _t = t + dt;
		integrate_adaptive(stepper, *m_breaker_system, x, _t, t0+T, 0.01, *m_breaker_observer);
		auto xT = m_breaker_observer->xT();
		if (m_breaker_observer->xT()[1] > 0) {
			t = t + dt;
			dt /= 2;
		} else {
			dt /= 2;
		}
		n++;

	} while (abs(m_breaker_observer->xT()[1]) > 0.1 && n < 10000);
	return std::make_tuple(trajectory(t+dt),t+dt);
}

std::tuple<odeint_2dstate, double>Breaker::burnStartLocTime(
		double t0,
		krpc::services::SpaceCenter::Orbit orbit,
		krpc::services::SpaceCenter::CelestialBody body)
{
	std::function<odeint_2dstate(double)> trajectory = std::bind(&Breaker::standardTrajectory, this, orbit, body, std::placeholders::_1);
	return burnStartLocTime(t0,trajectory);
}

std::tuple<odeint_2dstate, double>Breaker::burnStartLocTime(
		double t0,
		odeint_2dstate (*trajectory)(double))
{
	return burnStartLocTime(t0, std::function<odeint_2dstate(double)>(trajectory));
}
