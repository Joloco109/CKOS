#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <vector>
#include <functional>
namespace CKOS {

namespace Simulator {

	namespace ublas = boost::numeric::ublas;

	typedef boost::numeric::ublas::bounded_vector<double, 2> vector2;
	typedef boost::numeric::ublas::bounded_vector<double, 3> vector3;
	typedef boost::numeric::ublas::bounded_vector<double, 4> state2;
	typedef boost::numeric::ublas::bounded_vector<double, 6> state3;

	std::vector<state2> integrateOrbital2d(std::function<vector2(double)> F, double mu, state2 s0);
	std::vector<state3> integrate3d(std::function<vector3(double)> F, state3 s0);

}

}
