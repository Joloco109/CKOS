#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <tuple>
#include <cmath>

typedef std::tuple<double, double, double> vector3;
typedef std::tuple<float, float, float> vector3f;
typedef std::tuple<double, double> vector2;
typedef std::tuple<float, float> vector2f;

namespace math {
	using namespace boost::numeric::ublas;
	
	vector3 to_vector3(const bounded_vector<double,3>& in);
	bounded_vector<double,3> to_uvector3(const vector3& in);

	vector2 to_vector2(const bounded_vector<double,2>& in);
	bounded_vector<double,2> to_uvector2(const vector2& in);

	vector3f to_vector3f(const bounded_vector<float,3>& in);
	bounded_vector<float,3> to_uvector3f(const vector3f& in);

	vector2f to_vector2f(const bounded_vector<float,2>& in);
	bounded_vector<float,2> to_uvector2f(const vector2f& in);

	bounded_vector<float,2> to_pitch_heading (const bounded_vector<double,3> direction);

	double angle(const bounded_vector<double,3> v, const bounded_vector<double,3> w);
}
