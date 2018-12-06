#include "Math.h"

namespace k_math {
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
		out[0] = (float) angle(direction,horizon_direction);
		out[1] = (float) angle(unit_vector<double>(3,1),horizon_direction);
		if (direction[0] < 0)
			out[0] = -out[0];
		if (direction[2] < 0)
			out[1] = 360-out[1];
		return out;
	}

	double angle(const bounded_vector<double,3> v, const bounded_vector<double,3> w) {
		double dot = inner_prod(v,w);
		return std::acos(dot/(norm_2(v)*norm_2(w))) * 180 / M_PI;
	}

	bool testMath() {
		std::vector<double> v_up {1,0.001,0};
		std::vector<double> v_north {0,1,0};
		std::vector<double> v_northeast {0,1,1};
		std::vector<double> v_northup {1,1,0};
		std::vector<double> v_south {0,-1,0};
		std::vector<double> v_down {-1,0.001,0};

		bounded_vector<double, 3> up;
		bounded_vector<double, 3> north;
		bounded_vector<double, 3> northeast;
		bounded_vector<double, 3> northup;
		bounded_vector<double, 3> south;
		bounded_vector<double, 3> down;
		std::copy(v_up.begin(),v_up.end(), up.begin());
		std::copy(v_north.begin(),v_north.end(), north.begin());
		std::copy(v_northeast.begin(),v_northeast.end(), northeast.begin());
		std::copy(v_northup.begin(),v_northup.end(), northup.begin());
		std::copy(v_south.begin(),v_south.end(), south.begin());
		std::copy(v_down.begin(),v_down.end(), down.begin());

		std::cout << "up" << up << std::endl;
		std::cout << "north"<< north << std::endl;
		std::cout << "north-east"<< northeast << std::endl;
		std::cout << "north-up"<< northup << std::endl;


		std::cout << "up pitch_heading" << to_pitch_heading(up) << std::endl;
		std::cout << "north, pitch_heading" << to_pitch_heading(north) << std::endl;
		std::cout << "north-east, pitch_heading" << to_pitch_heading(northeast) << std::endl;
		std::cout << "north-up, pitch_heading" << to_pitch_heading(northup) << std::endl;
		std::cout << "south, pitch_heading" << to_pitch_heading(south) << std::endl;
		std::cout << "down, pitch_heading" << to_pitch_heading(down) << std::endl;
		return true;
	}

	std::ostream& operator<<(std::ostream& out, const vector<double>& v) {
		auto it = v.begin();
		out << "(";
		while (it != v.end()) {
			out << it.operator*();
			it++;
			if (it != v.end())
				out << ", ";
		}
		out << ")";
		return out;
	}

	std::string to_string(const vector <double>& v) {
		std::ostringstream s;
		s << v;
		return s.str();
	}
}
