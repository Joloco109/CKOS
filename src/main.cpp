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
	std::shared_ptr<Logger> logger = std::make_shared<Logger>();

	krpc::Client* conn = new krpc::Client(krpc::connect("C++ main test"));
	krpc::services::KRPC* krpc = new krpc::services::KRPC(conn);
	//std::cout << "Connected to kRPC server version " << krpc->get_status().version() << std::endl;
	logger->log("Connected to kRPC server version " + krpc->get_status().version(), LogLevel::Norminal);

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

	//std::cout << "Time = " << plan.current()->getInfo()->ut->operator()() << std::endl;
	logger->log("Time = " + std::to_string(plan.current()->getInfo()->ut->operator()()), LogLevel::Norminal);
	//std::cout << "Current Stage: " << plan.current()->getName() << std::endl;
	logger->log("Current Stage: " + plan.current()->getName(), LogLevel::Norminal);
	plan.update();
	while (plan.update());

	std::cin.get();
}

int main() {
	try {
		testConn();
	}
	catch (krpc::ConnectionError& e) {
		std::cerr << "Error in Connection!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
	catch (krpc::RPCError& e) {
		std::cerr << "RPC error in Test!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
	catch (std::system_error& e) {
		std::cerr << "System error in Test!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
}
