#include "Math.h"

namespace math {
	using namespace boost::numeric::ublas;
	
	vector3 to_vector3(const bounded_vector<double,3>& in) {
		return vector3(in[0],in[1],in[2]);
	}

	bounded_vector<double,3> to_uvector3(const vector3& in) {
		bounded_vector<double,3> out;
		out[0] = std::get<0>(in);
		out[1] = std::get<1>(in);
		out[2] = std::get<2>(in);
		return out;
	}

	vector2 to_vector2(const bounded_vector<double,2>& in) {
		return vector2(in[0],in[1]);
	}

	bounded_vector<double,2> to_uvector2(const vector2& in) {
		bounded_vector<double,3> out;
		out[0] = std::get<0>(in);
		out[1] = std::get<1>(in);
		return out;
	}

	vector3f to_vector3f(const bounded_vector<float,3>& in) {
		return vector3f(in[0],in[1],in[2]);
	}

	bounded_vector<float,3> to_uvector3f(const vector3f& in) {
		bounded_vector<double,3> out;
		out[0] = std::get<0>(in);
		out[1] = std::get<1>(in);
		out[2] = std::get<2>(in);
		return out;
	}

	vector2f to_vector2f(const bounded_vector<float,2>& in) {
		return vector2f(in[0],in[1]);
	}

	bounded_vector<float,2> to_uvector2f(const vector2f& in) {
		bounded_vector<double,3> out;
		out[0] = std::get<0>(in);
		out[1] = std::get<1>(in);
		return out;
	}

	bounded_vector<float,2> to_pitch_heading (const bounded_vector<double,3> direction) {
		bounded_vector<float,2> out;
		bounded_vector<double, 3> horizon_direction = direction - direction[0] * unit_vector<double>(3,0);
		out[0] = (float) angle(horizon_direction,direction);
		out[1] = (float) angle(horizon_direction,unit_vector<double>(3,1));
		if (direction[0] < 0)
			out[0] = -out[0];
		if (direction[2] < 0)
			out[1] = -out[1];
		return out;
	}

	double angle(const bounded_vector<double,3> v, const bounded_vector<double,3> w) {
		double dot = inner_prod(v,w);
		return std::acos(dot/(norm_2(v)*norm_2(w))) * 180 / M_PI;
	}
}
