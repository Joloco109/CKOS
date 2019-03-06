#pragma once

#include <boost/numeric/odeint.hpp>
#include "../Interface.h"

namespace CKOS {

namespace Simulator {

namespace integrate {
	
	class Orbital2dSystem {
		std::function<vector2(double)> m_F;
		double m_mu;
		
	public:
		Orbital2dSystem(std::function<vector2(double)> F, double mu);

		void operator() (const state2 &x, state2 &dxdt, const double t);
	};
}

}

}
