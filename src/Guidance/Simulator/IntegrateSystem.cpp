#include "IntegrateSystem.h"

namespace CKOS {

namespace Simulator {

namespace integrate {

	Orbital2dSystem::Orbital2dSystem(std::function<vector2(double)> F, double mu)
		: m_F(F) 
		, m_mu(mu)
	{ }

	void Orbital2dSystem::operator() (const state2 &x, state2 &dxdt, const double t) {
		auto f = m_F(t);
		dxdt[0] = x[2];
		dxdt[1] = x[3];
		dxdt[2] = m_mu / pow(x[0], 2) - pow(x[3], 2) * x[0] + f[0];
		dxdt[3] = (-2*x[2]*x[3] + f[1]) / x[0];
	}
}

}

}
