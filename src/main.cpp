#include <iostream>
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>
#include "Logger.h"
#include "MissionPlan.h"
#include "MissionInfo.h"
#include "AscentStage.h"
#include <memory>

void testConn() {
	krpc::Client* conn = new krpc::Client(krpc::connect("C++ main test"));
	krpc::services::KRPC* krpc = new krpc::services::KRPC(conn);
	std::cout << "Connected to kRPC server version " << krpc->get_status().version() << std::endl;

	using krpc::services::SpaceCenter;

	SpaceCenter* space_center = new SpaceCenter(conn);
	SpaceCenter::Vessel* vessel = new SpaceCenter::Vessel(space_center->active_vessel());

	auto info = std::make_shared<MissionInfo>(
				std::shared_ptr<SpaceCenter>(space_center), 
				std::shared_ptr<SpaceCenter::Vessel>(vessel));
	MissionPlaner plan{info};
	std::shared_ptr<MissionStage> firstStage = std::make_shared<AscentStage>("First Stage", 
		info);
	plan.addStage(firstStage);

	std::cout << "Time = " << plan.current()->getInfo()->ut->operator()() << std::endl;
	std::cout << "Current Stage: " << plan.current()->getName() << std::endl;
	plan.update();
	while (plan.update());

	std::cin.get();
}

int main() {
	try {
		testConn();
	}
	catch (krpc::ConnectionError& e) {
		std::cout << "Error in Connection!" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}
	catch (krpc::RPCError& e) {
		std::cout << "RPC error in Test!" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}
	catch (std::system_error& e) {
		std::cout << "System error in Test!" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}
}
