#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <tuple>
#include <cmath>

#define boost_uvector3 boost::numeric::ublas::bounded_vector<double, 3>
#define boost_uvector2 boost::numeric::ublas::bounded_vector<double, 2>
#define boost_uvector3f boost::numeric::ublas::bounded_vector<float, 3>
#define boost_uvector2f boost::numeric::ublas::bounded_vector<float, 2>

typedef std::tuple<double, double, double> vector3;
typedef std::tuple<float, float, float> vector3f;
typedef std::tuple<double, double> vector2;
typedef std::tuple<float, float> vector2f;

namespace k_math {
	using namespace boost::numeric::ublas;
	
	vector3 to_vector3(const boost_uvector3& in);
	boost_uvector3 to_uvector3(const vector3& in);

	vector2 to_vector2(const boost_uvector2& in);
	boost_uvector2 to_uvector2(const vector2& in);

	vector3f to_vector3f(const boost_uvector3f& in);
	boost_uvector3f to_uvector3f(const vector3f& in);

	vector2f to_vector2f(const boost_uvector2f& in);
	boost_uvector2f to_uvector2f(const vector2f& in);

	boost_uvector2f to_pitch_heading (const boost_uvector3 direction);

	double angle(const boost_uvector3 v, const boost_uvector3 w);

	bool testMath();

	std::ostream& operator<<(std::ostream& out, const vector<double>& v);

	std::string to_string(const vector<double>& v);
}
