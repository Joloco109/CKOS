#include "Interface.h"
#include <memory>


namespace CKOS {

namespace Simulator {
	std::vector<state2> integrate2d(std::function<vector2(double)> F, state2 s0) {
		std::vector<state2> out;
		return out;
	}

	std::vector<state3> integrate3d(std::function<vector3(double)> F, state3 s0) {
		return std::vector<state3>();
	}

}

}
