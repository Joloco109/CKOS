#include "Interface.h"
#include <memory>
#include <Simulator/IntegrateSystem.h>
#include <boost/numeric/odeint.hpp>

namespace CKOS {

namespace Simulator {
	std::vector<state2> integrateOrbital2d(std::function<vector2(double)> F, double mu, state2 s0) {
		std::vector<state2> out;
		integrate::Orbital2dSystem system(F, mu);
		using namespace boost::numeric::odeint;
		auto stepper = make_dense_output(1.0e-3, 1.0e-3, runge_kutta_dopri5<state2>());
		//integrate_adaptive(stepper, system, s0, 0, 100);
		return out;
	}

	std::vector<state3> integrate3d(std::function<vector3(double)> F, state3 s0) {
		return std::vector<state3>();
	}

}

}
